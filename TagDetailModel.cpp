#include "TagDetailModel.h"
#include "TextBlock.h"
#include <QSet>
#include <QFile>
#include <QTextStream>
#include <QDir>

TagInstanceModel::TagInstanceModel(const QString& tag, QObject* parent)
    : QStandardItemModel(parent), _keyword(tag)
{
    setColumnCount(3);
    setHeaderData(COL_FILEPATH, Qt::Horizontal, tr("File"));
    setHeaderData(COL_LINENUM,  Qt::Horizontal, tr("Line"));
    setHeaderData(COL_CONTENT,  Qt::Horizontal, tr("Comment"));
}

TagInstanceModel* TagInstanceModel::pick(int n)
{
    if(n >= rowCount())   // pick all
        return this;

    TagInstanceModel* result = new TagInstanceModel(getKeyword(), parent());  // a new model
    if(n <= 0)            // pick 0, return an empty model
        return result;

    int pickCount = qMin(n, rowCount());
    QSet<int> pickedRow;
    int i = 0;
    while(i < pickCount)
    {
        int row = qrand() % rowCount();   // random row
        if(!pickedRow.contains(row))      // copy row
        {
            int lastRow = result->rowCount();
            result->insertRow(lastRow);
            result->setData(result->index(lastRow, COL_FILEPATH), data(index(row, COL_FILEPATH)));
            result->setData(result->index(lastRow, COL_LINENUM),  data(index(row, COL_LINENUM)));
            result->setData(result->index(lastRow, COL_CONTENT),  data(index(row, COL_CONTENT)));

            pickedRow.insert(row);
            ++i;
        }
    }
    return result;
}

void TagInstanceModel::save(const QString& filePath, const QString& projectPath)
{
    QFile file(filePath);
    if(file.open(QFile::WriteOnly))
    {
        QTextStream os(&file);
        for(int row = 0; row < rowCount(); ++row)
        {
            QString filePath = getFilePath(row);                // absolute path
            filePath.remove(projectPath, Qt::CaseInsensitive);  // to relative path
            os << filePath << ","
               << getLineNum(row) << ","
               << getContent(row) << _lineSeparator;
        }
    }
}

QList<TextBlock> TagInstanceModel::load(const QString& filePath, const QString& projectPath)
{
    QList<TextBlock> result;
    QFile file(filePath);
    if(file.open(QFile::ReadOnly))
    {
        QTextStream is(&file);
        QStringList tags = is.readAll().split(_lineSeparator);  // each line is a tag
        foreach(const QString& tag, tags)
        {
            QStringList sections = tag.split(",");   // 3 sections: path, line, content
            if(sections.size() < 3)
                continue;

            result << TextBlock(sections.at(COL_CONTENT),
                                projectPath + QDir::toNativeSeparators(sections.at(COL_FILEPATH)),
                                sections.at(COL_LINENUM).toInt());
        }
    }
    return result;
}

QString TagInstanceModel::getFilePath(int row) const {
    return QDir::toNativeSeparators(data(index(row, COL_FILEPATH)).toString());
}
int TagInstanceModel::getLineNum(int row) const {
    return data(index(row, COL_LINENUM)).toInt();
}
QString TagInstanceModel::getContent(int row) const {
    return data(index(row, COL_CONTENT)).toString();
}

void TagInstanceModel::addTextBlock(const TextBlock& textBlock)
{
}

// do not use a single return to separate lines, because some tag contents contain returns
QString TagInstanceModel::_lineSeparator = ";;;\r\n";
