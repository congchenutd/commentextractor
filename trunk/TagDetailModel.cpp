#include "TagDetailModel.h"
#include "TextBlock.h"
#include <QSet>
#include <QFile>
#include <QTextStream>
#include <QDir>

TagDetailModel::TagDetailModel(const QString& tag, QObject* parent)
    : QStandardItemModel(parent), _tag(tag)
{
    setColumnCount(3);
    setHeaderData(COL_FILEPATH, Qt::Horizontal, tr("File"));
    setHeaderData(COL_LINENUM,     Qt::Horizontal, tr("Line"));
    setHeaderData(COL_CONTENT,  Qt::Horizontal, tr("Comment"));
}

TagDetailModel* TagDetailModel::pick(int n)
{
    if(n >= rowCount())
        return this;

    TagDetailModel* result = new TagDetailModel(getTag(), parent());
    if(n <= 0)
        return result;

    int pickCount = qMin(n, rowCount());
    QSet<int> pickedRow;
    int i = 0;
    while(i < pickCount)
    {
        int row = qrand() % rowCount();   // random row
        if(!pickedRow.contains(row))
        {
            // copy row
            int lastRow = result->rowCount();
            result->insertRow(lastRow);
            result->setData(result->index(lastRow, COL_FILEPATH), data(index(row, COL_FILEPATH)));
            result->setData(result->index(lastRow, COL_LINENUM),     data(index(row, COL_LINENUM)));
            result->setData(result->index(lastRow, COL_CONTENT),  data(index(row, COL_CONTENT)));

            pickedRow.insert(row);
            ++i;
        }
    }
    return result;
}

void TagDetailModel::save(const QString& filePath, const QString& sourcePath)
{
    QFile file(filePath);
    if(file.open(QFile::WriteOnly))
    {
        QTextStream os(&file);
        for(int row = 0; row < rowCount(); ++row)
        {
            QString filePath = QDir::toNativeSeparators(
                                         data(index(row, COL_FILEPATH)).toString());
            filePath.remove(sourcePath, Qt::CaseInsensitive);
            os << filePath << ","
               << data(index(row, COL_LINENUM))   .toString() << ","
               << data(index(row, COL_CONTENT)).toString() << _lineSeparator;
        }
    }
}

QList<TextBlock> TagDetailModel::fromFile(const QString& filePath, const QString& projectPath)
{
    QList<TextBlock> result;
    QFile file(filePath);
    if(file.open(QFile::ReadOnly))
    {
        QTextStream is(&file);
        QString content = is.readAll();
        QStringList tags = content.split(_lineSeparator);
        foreach(const QString& tag, tags)
        {
            QStringList sections = tag.split(",");
            if(sections.size() < 3)
                continue;

            QString relativePath = QDir::toNativeSeparators(sections.at(COL_FILEPATH));
            QString filePath = projectPath + QDir::separator() + relativePath;
            result << TextBlock(sections.at(COL_CONTENT),
                                filePath,
                                sections.at(COL_LINENUM).toInt());
        }
    }
    return result;
}

// do not use a single return to separate lines, because some tag contents contain returns
QString TagDetailModel::_lineSeparator = ";;;\r\n";
