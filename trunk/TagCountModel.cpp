#include "TagCountModel.h"
#include "TagDetailModel.h"
#include "TextBlock.h"
#include "Counter.h"
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QApplication>

TagCountModel::TagCountModel(QObject* parent, TagCounter* counter)
    : QStandardItemModel(parent), _counter(counter)
{
    setColumnCount(2);
    setHeaderData(COL_TAG,   Qt::Horizontal, tr("Keyword"));
    setHeaderData(COL_COUNT, Qt::Horizontal, tr("Count"));
}

void TagCountModel::setProjectPath(const QString &projectPath) {
    _projectPath = QDir::toNativeSeparators(projectPath);
}

void TagCountModel::addTag(const QString& tag, const TextBlock& block)
{
    TagInstanceModel* detailModel = 0;
    int row = findTag(tag);
    if(row == -1)  // new tag
    {
        int lastRow = rowCount();
        insertRow(lastRow);
        setData(index(lastRow, COL_TAG),   tag);
        setData(index(lastRow, COL_COUNT), 1);

        detailModel = new TagInstanceModel(tag, this);
        _keyword2Model.insert(tag, detailModel);
    }
    else           // existing tag
    {
        setData(index(row, COL_TAG), tag);
        setData(index(row, COL_COUNT), data(index(row, COL_COUNT)).toInt() + 1);
        detailModel = getInstanceModel(tag);
    }

    // add to the detail model
    if(detailModel != 0)
        detailModel->addTextBlock(block);

    if(_counter != 0)
        _counter->increase();
}

TagInstanceModel* TagCountModel::getInstanceModel(const QString& tag) const {
    return _keyword2Model.contains(tag) ? _keyword2Model[tag] : 0;
}

void TagCountModel::clear()
{
    removeRows(0, rowCount());
    foreach(TagInstanceModel* detailModel, _keyword2Model)
        detailModel->deleteLater();
    _keyword2Model.clear();
}

void TagCountModel::pick(int n)
{
    for(Keyword2Model::Iterator it = _keyword2Model.begin(); it != _keyword2Model.end(); ++it)
    {
        TagInstanceModel* newModel = it.value()->pick(n);  // a new model containing n picked instances
        if(newModel != it.value())
            it.value()->deleteLater();                     // delete the old
        *it = newModel;                                    // use the new model
    }
}

void TagCountModel::removeSmall(int n)
{
    if(n <= 0)
        return;

    for(int row = 0; row < rowCount();)
    {
        if(getCount(row) < n)
            remove(row);
        else
            row ++;
    }
}

void TagCountModel::remove(const QString& tag) {
    remove(findTag(tag));
}

void TagCountModel::remove(int row)
{
    if(row < 0)
        return;

    QString tag = getKeyword(row);
    getInstanceModel(tag)->deleteLater();
    _keyword2Model.remove(tag);

    if(_counter != 0)
        _counter->decrease(getCount(row));

    removeRow(row);
}

QString TagCountModel::getModuleName(const TextBlock& textBlock, const QString& modularity) const {
    return modularity == "PACKAGE" ? textBlock.getPackageName()
                                   : modularity == "FILE" ? textBlock.getFilePath()
                                                          : textBlock.getCompleteClassName();
}

void TagCountModel::save(const QString& dirPath)
{
    QFile file(dirPath + QDir::separator() + "TagCount.csv");
    if(file.open(QFile::WriteOnly))
    {
        QTextStream os(&file);
        os << _projectPath << "\r\n";   // save project path
        for(int row = 0; row < rowCount(); ++row)
        {
            QString tag = getKeyword(row);
            os << tag << "," << getCount(row) << "\r\n";   // save tag count

            // save tag detail to a separate file
            getInstanceModel(tag)->save(dirPath + QDir::separator() + tag + ".csv", _projectPath);
        }
    }
}

void TagCountModel::load(const QString& dirPath)
{
    clear();

    QFile file(dirPath + QDir::separator() + "TagCount.csv");
    if(file.open(QFile::ReadOnly))
    {
        QTextStream is(&file);
        setProjectPath(is.readLine());   // load project path

        while(!is.atEnd())
        {
            QStringList sections = is.readLine().split(",");
            if(sections.size() != 2)
                continue;

            // load tag detail
            QString tag = sections.at(COL_TAG);
            QList<TextBlock> blocks = TagInstanceModel::load(dirPath + QDir::separator() + tag + ".csv",
                                                             _projectPath);
            foreach(const TextBlock& block, blocks)
                addTag(tag, block);
            qApp->processEvents();
        }
    }
}

void TagCountModel::exportToFile(const QString& filePath, const QString& modularity)
{
    TagDistributionModel* distributionModel = new TagDistributionModel(_keyword2Model.keys());
    foreach(TagInstanceModel* instanceModel, _keyword2Model)
    {
        QList<TextBlock> textBlocks = instanceModel->getTextBlocks();
        foreach(const TextBlock& textBlock, textBlocks)
            distributionModel->addCount(getModuleName(textBlock, modularity),
                                        instanceModel->getKeyword());
    }
    distributionModel->exportToFile(filePath);
}

int TagCountModel::findTag(const QString& tag) const
{
    QModelIndexList indexes = match(index(0, COL_TAG), Qt::DisplayRole, tag, 1, Qt::MatchExactly);
    return indexes.isEmpty() ? -1 : indexes.front().row();
}

QString TagCountModel::getKeyword(int row) const {
    return data(index(row, COL_TAG)).toString();
}

int TagCountModel::getCount(int row) const {
    return data(index(row, COL_COUNT)).toInt();
}

///////////////////////////////////////////////////////////////////////
TagDistributionModel::TagDistributionModel(const QStringList& keywords, QObject* parent)
    : QStandardItemModel(parent)
{
    setColumnCount(keywords.size());
    for(int col = 0; col < columnCount(); ++col)
    {
        setHeaderData(col, Qt::Horizontal, keywords.at(col));
        _keyword2Col.insert(keywords.at(col), col);
    }
}

void TagDistributionModel::addCount(const QString& moduleName, const QString& keyword)
{
//    qDebug() << moduleName;

    int col = findKeywordCol(keyword);
    if(col == -1)
        return;

    int row = findPackageRow(moduleName);
    if(row == -1)       // new row
    {
        row = rowCount();
        insertRow(row);
        setHeaderData(row, Qt::Vertical, moduleName);
        _package2Row.insert(moduleName, row);
    }

    setData(index(row, col), data(index(row, col)).toInt() + 1);
}

void TagDistributionModel::exportToFile(const QString& filePath)
{
    QFile file(filePath);
    if(!file.open(QFile::WriteOnly))
        return;

    // first row is the table header
    QTextStream os(&file);
    for(int col = 0; col < columnCount(); ++col)
        os << "," << headerData(col, Qt::Horizontal).toString();
    os << "\r\n";

    for(int row = 0; row < rowCount(); ++row)
    {
        os << headerData(row, Qt::Vertical).toString() << ",";  // vertical header
        for(int col = 0; col < columnCount(); ++col)
            os << data(index(row, col)).toInt() << ",";
        os << "\r\n";
    }
}

int TagDistributionModel::findKeywordCol(const QString& keyword) const {
    return _keyword2Col.contains(keyword) ? _keyword2Col[keyword] : -1;
}

int TagDistributionModel::findPackageRow(const QString& packageName) const {
    return _package2Row.contains(packageName) ? _package2Row[packageName] : -1;
}
