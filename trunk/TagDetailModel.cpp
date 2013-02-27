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
    setHeaderData(COL_LINE,     Qt::Horizontal, tr("Line"));
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
    QSet<int> picked;
    int i = 0;
    while(i < pickCount)
    {
        int row = qrand() % rowCount();   // random row
        if(!picked.contains(row))
        {
            // copy row
            int lastRow = result->rowCount();
            result->insertRow(lastRow);
            result->setData(result->index(lastRow, COL_FILEPATH), data(index(row, COL_FILEPATH)));
            result->setData(result->index(lastRow, COL_LINE),     data(index(row, COL_LINE)));
            result->setData(result->index(lastRow, COL_CONTENT),  data(index(row, COL_CONTENT)));

            picked.insert(row);
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
               << data(index(row, COL_LINE))    .toString() << ","
               << data(index(row, COL_CONTENT)) .toString() << ";;;\r\n";
            // do not use return to separate lines, because some tag contents contain returns
        }
    }
}

QList<TextBlock> TagDetailModel::fromFile(const QString& filePath, const QString& sourcePath)
{
    QList<TextBlock> result;
    QFile file(filePath);
    if(file.open(QFile::ReadOnly))
    {
        QTextStream is(&file);
        QString content = is.readAll();
        QStringList tags = content.split(";;;\r\n");
        foreach(const QString& tag, tags)
        {
            QStringList sections = tag.split(",");
            if(sections.size() < 3)
                continue;

            QString filePath = sourcePath + QDir::separator()
                                          + QDir::toNativeSeparators(sections.at(COL_FILEPATH));
            result << TextBlock(sections.at(COL_CONTENT),
                                filePath,
                                sections.at(COL_LINE).toInt());
        }
    }
    return result;
}
