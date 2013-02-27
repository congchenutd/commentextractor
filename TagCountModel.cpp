#include "TagCountModel.h"
#include "TagDetailModel.h"
#include "TextBlock.h"
#include "Counter.h"
#include <QTextStream>
#include <QFile>
#include <QDir>

TagCountModel::TagCountModel(QObject* parent, TagCounter* counter)
    : QStandardItemModel(parent), _counter(counter)
{
    setColumnCount(2);
    setHeaderData(COL_TAG,   Qt::Horizontal, tr("Keyword"));
    setHeaderData(COL_COUNT, Qt::Horizontal, tr("Count"));
}

void TagCountModel::addTag(const QString& tag, const TextBlock& block)
{
    TagDetailModel* detailModel = 0;
    int row = findTag(tag);
    if(row == -1)  // new tag
    {
        int lastRow = rowCount();
        insertRow(lastRow);
        setData(index(lastRow, COL_TAG),   tag);
        setData(index(lastRow, COL_COUNT), 1);

        detailModel = new TagDetailModel(tag, this);
        _details.insert(tag, detailModel);
    }
    else           // existing tag
    {
        setData(index(row, COL_TAG), tag);
        setData(index(row, COL_COUNT), data(index(row, COL_COUNT)).toInt() + 1);
        detailModel = getDetail(tag);
    }

    // add to the detail model
    if(detailModel != 0)
    {
        int lastRow = detailModel->rowCount();
        detailModel->insertRow(lastRow);
        detailModel->setData(detailModel->index(lastRow, TagDetailModel::COL_FILEPATH), block.getFilePath());
        detailModel->setData(detailModel->index(lastRow, TagDetailModel::COL_LINE),     block.getLineNumber());
        detailModel->setData(detailModel->index(lastRow, TagDetailModel::COL_CONTENT),  block.getContent());
    }

    if(_counter)
        _counter->increase();
}

TagDetailModel* TagCountModel::getDetail(const QString& tag) const {
    return _details.contains(tag) ? _details[tag] : 0;
}

void TagCountModel::clear()
{
    removeRows(0, rowCount());
    foreach(TagDetailModel* detailModel, _details)
        detailModel->deleteLater();
    _details.clear();
}

void TagCountModel::pick(int n)
{
    for(QMap<QString, TagDetailModel*>::Iterator it = _details.begin(); it != _details.end(); ++it)
    {
        TagDetailModel* newModel = it.value()->pick(n);  // a new model containing n picked instances
        if(newModel != it.value())
            it.value()->deleteLater();                   // delete the old
        *it = newModel;                                  // use the new model
    }
}

void TagCountModel::removeSmall(int n)
{
    if(n <= 0 || n >= rowCount())
        return;

    for(int row = 0; row < rowCount();) {
        if(data(index(row, COL_COUNT)).toInt() < n)
            remove(data(index(row, COL_TAG)).toString());
        else
            row ++;
    }
}

void TagCountModel::remove(const QString& tag)
{
    int row = findTag(tag);
    if(row == -1)
        return;

    if(_counter)
        _counter->decrease(getDetail(tag)->rowCount());

    getDetail(tag)->deleteLater();
    _details.remove(tag);
    removeRow(row);
}

void TagCountModel::save(const QString& dirPath, const QString& sourcePath)
{
    QFile file(dirPath + QDir::separator() + "TagCount.csv");
    if(file.open(QFile::WriteOnly))
    {
        QTextStream os(&file);
        os << sourcePath << "\r\n";   // save source path
        for(int row = 0; row < rowCount(); ++row)
        {
            QString tag = data(index(row, COL_TAG)).toString();
            os << tag << "," << data(index(row, COL_COUNT)).toString() << "\r\n";   // save tag count
            getDetail(tag)->save(dirPath + QDir::separator() + tag + ".csv",
                                 sourcePath);       // save tag detail
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
        QString sourcePath = QDir::toNativeSeparators(is.readLine());   // load source path

        while(!is.atEnd())
        {
            QStringList sections = is.readLine().split(",");
            if(sections.size() != 2)
                continue;

            // load tag detail
            QString tag = sections.at(COL_TAG);
            QList<TextBlock> blocks = TagDetailModel::fromFile(dirPath + QDir::separator() + tag + ".csv",
                                                               sourcePath);
            foreach(const TextBlock& block, blocks)
                addTag(tag, block);
        }
    }
}

int TagCountModel::findTag(const QString& tag) const
{
    QModelIndexList indexes = match(index(0, COL_TAG), Qt::DisplayRole, tag, 1, Qt::MatchExactly);
    return indexes.isEmpty() ? -1 : indexes.front().row();
}
