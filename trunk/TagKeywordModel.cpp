#include "TagKeywordModel.h"
#include "TagInstanceModel.h"
#include "TextBlock.h"
#include "Counter.h"
#include "Settings.h"
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QApplication>
#include <QDebug>

TagKeywordModel::TagKeywordModel(TagCounter*       counterTag,
                                 TagLineCounter*   counterTagLine,
                                 TagLengthCounter* counterTagLen,
                                 QObject* parent)
    : QStandardItemModel(parent),
      _counterTag    (counterTag),
      _counterTagLine(counterTagLine),
      _counterTagLen (counterTagLen)
{
    setColumnCount(4);
    setHeaderData(COL_KEYWORD,   Qt::Horizontal, tr("Keyword"));
    setHeaderData(COL_COUNT,     Qt::Horizontal, tr("Count"));
    setHeaderData(COL_LINECOUNT, Qt::Horizontal, tr("Lines"));
    setHeaderData(COL_LEN,       Qt::Horizontal, tr("Length"));
}

void TagKeywordModel::addTag(const QString& keyword, const TextBlock& block)
{
    TagInstanceModel* instanceModel = 0;
    int lineCount = block.getContent().count("\n") + 1;
    int wordCount = block.getContent().count(QRegExp("\\b\\w+\\b"));
    int row = findKeyword(keyword);
    if(row == -1)  // new kw
    {
        int lastRow = rowCount();
        insertRow(lastRow);
        setData(index(lastRow, COL_KEYWORD),   keyword);
        setData(index(lastRow, COL_COUNT),     1);
        setData(index(lastRow, COL_LINECOUNT), lineCount);
        setData(index(lastRow, COL_LEN),       wordCount);

        instanceModel = new TagInstanceModel(keyword, this);
        _keyword2Model.insert(keyword, instanceModel);
    }
    else           // existing kw
    {
        setData(index(row, COL_COUNT),     getCount(row)     + 1);
        setData(index(row, COL_LINECOUNT), getLineCount(row) + lineCount);
        setData(index(row, COL_LEN),       getLength   (row) + wordCount);
        instanceModel = getInstanceModel(keyword);
    }

    // add to the instance model
    if(instanceModel != 0)
        instanceModel->addTextBlock(block);

    _counterTag    ->increase();
    _counterTagLine->increase(lineCount);
    _counterTagLen ->increase(wordCount);
}

TagInstanceModel* TagKeywordModel::getInstanceModel(const QString& tag) const {
    return _keyword2Model.contains(tag) ? _keyword2Model[tag] : 0;
}

void TagKeywordModel::clear()
{
    removeRows(0, rowCount());
    foreach(TagInstanceModel* instanceModel, _keyword2Model)
        instanceModel->deleteLater();
    _keyword2Model.clear();

    _counterTag    ->reset();
    _counterTagLine->reset();
    _counterTagLen ->reset();
}

void TagKeywordModel::pick(int n)
{
    // for each keyword, pick n random instances
    for(Keyword2Model::Iterator it = _keyword2Model.begin(); it != _keyword2Model.end(); ++it)
    {
        TagInstanceModel* newModel = it.value()->pick(n);  // a new model containing n picked instances
        if(newModel != it.value())
            it.value()->deleteLater();                     // delete the old
        *it = newModel;                                    // use the new model
    }
}

void TagKeywordModel::removeSmall(int n)
{
    if(n <= 0)
        return;

    for(int row = 0; row < rowCount();)
    {
        if(getCount(row) < n)   // remove small keyword
            removeRow(row);
        else
            row ++;
    }
}

void TagKeywordModel::removeKeyword(const QString& keyword) {
    removeRow(findKeyword(keyword));
}

void TagKeywordModel::removeRow(int row)
{
    if(row < 0 || row >= rowCount())
        return;

    QString keyword = getKeyword(row);
    getInstanceModel(keyword)->deleteLater();
    _keyword2Model.remove(keyword);

    _counterTag    ->decrease(getCount(row));
    _counterTagLine->decrease(getLineCount(row));
    _counterTagLen ->decrease(getLength(row));

    QStandardItemModel::removeRow(row);
}

QString TagKeywordModel::getModuleName(const TextBlock& textBlock, const QString& modularity) const {
    return modularity == "PACKAGE" ? textBlock.getPackageName()
                                   : modularity == "FILE" ? textBlock.getFilePath()
                                                          : textBlock.getCompleteClassName();
}

void TagKeywordModel::save(const QString& dirPath)
{
    QFile file(dirPath + QDir::separator() + "TagCount.csv");
    if(file.open(QFile::WriteOnly))
    {
        QTextStream os(&file);
        os << Settings().getProjectPath() << "\r\n";   // save project path
        for(int row = 0; row < rowCount(); ++row)
        {
            QString keyword = getKeyword(row);
            os << keyword << "," << getCount(row) << "\r\n";   // save tag count

            // save tag instances to a separate file
            getInstanceModel(keyword)->save(dirPath + QDir::separator() + keyword + ".csv");
        }
    }
}

void TagKeywordModel::load(const QString& dirPath)
{
    clear();

    QFile file(dirPath + QDir::separator() + "TagCount.csv");
    if(file.open(QFile::ReadOnly))
    {
        QTextStream is(&file);
        Settings().setProjectPath(is.readLine());

        while(!is.atEnd())
        {
            QStringList sections = is.readLine().split(",");
            if(sections.size() != 2)
                continue;

            // load tag instances
            QString keyword = sections.at(COL_KEYWORD);
            QList<TextBlock> blocks = TagInstanceModel::load(dirPath + QDir::separator() + keyword + ".csv",
                                                             Settings().getProjectPath());
            foreach(const TextBlock& block, blocks)
                addTag(keyword, block);
            qApp->processEvents();
        }
    }
}

void TagKeywordModel::exportToFile(const QString& filePath, const QString& modularity)
{
    // write all tags to a table
    TagDistributionModel* distributionModel = new TagDistributionModel(_keyword2Model.keys());
    foreach(TagInstanceModel* instanceModel, _keyword2Model)
    {
        QList<TextBlock> textBlocks = instanceModel->getAllTextBlocks();
        foreach(const TextBlock& textBlock, textBlocks)
            distributionModel->addCount(getModuleName(textBlock, modularity),
                                        instanceModel->getKeyword());
    }

    // save the table to a file
    distributionModel->exportToFile(filePath);
}

int TagKeywordModel::findKeyword(const QString& keyword) const
{
    QModelIndexList indexes = match(index(0, COL_KEYWORD), Qt::DisplayRole, keyword, 1, Qt::MatchExactly);
    return indexes.isEmpty() ? -1 : indexes.front().row();
}

QString TagKeywordModel::getKeyword(int row) const {
    return data(index(row, COL_KEYWORD)).toString();
}

int TagKeywordModel::getCount(int row) const {
    return data(index(row, COL_COUNT)).toInt();
}

int TagKeywordModel::getLineCount(int row) const {
    return data(index(row, COL_LINECOUNT)).toInt();
}

int TagKeywordModel::getLength(int row) const {
    return data(index(row, COL_LEN)).toInt();
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
        os << headerData(row, Qt::Vertical).toString() << ",";  // vertical header is module name
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
