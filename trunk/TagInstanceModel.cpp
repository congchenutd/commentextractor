#include "TagInstanceModel.h"
#include "TextBlock.h"
#include "Settings.h"
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
            result->addTextBlock(getTextBlock(row));
            pickedRow << row;
            ++i;
        }
    }
    return result;
}

void TagInstanceModel::save(const QString& filePath)
{
    QFile file(filePath);
    if(file.open(QFile::WriteOnly))
    {
        QTextStream os(&file);
        for(int row = 0; row < rowCount(); ++row)
        {
            QString filePath = getFilePath(row);                                // absolute path
            filePath.remove(Settings().getProjectPath(), Qt::CaseInsensitive);  // to relative path
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
        QStringList lines = is.readAll().split(_lineSeparator);  // each line is a tag
        foreach(const QString& line, lines)
        {
            if(line.isEmpty())
                continue;

            QString filePath = line.section(",", 0, 0);
            int     lineNum  = line.section(",", 1, 1).toInt();
            QString content  = line.section(",", 2, -1);
            result << TextBlock(content,
                                projectPath + QDir::toNativeSeparators(filePath),
                                lineNum);
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

TextBlock TagInstanceModel::getTextBlock(int row) const {
    return TextBlock(getContent(row), getFilePath(row), getLineNum(row));
}

QList<TextBlock> TagInstanceModel::getAllTextBlocks() const
{
    QList<TextBlock> result;
    for(int row = 0; row < rowCount(); ++row)
        result << getTextBlock(row);
    return result;
}

void TagInstanceModel::setTextBlock(int row, const TextBlock& textBlock)
{
    if(row < 0 || row >= rowCount())
        return;
    setData(index(row, COL_CONTENT),  textBlock.getContent());
    setData(index(row, COL_FILEPATH), textBlock.getFilePath());
    setData(index(row, COL_LINENUM),  textBlock.getLineNumber());
}

void TagInstanceModel::addTextBlock(const TextBlock& textBlock)
{
    int lastRow = rowCount();
    insertRow(lastRow);
    setTextBlock(lastRow, textBlock);
}

// do not use a single return to separate lines, because some tag contents contain returns
QString TagInstanceModel::_lineSeparator = ";;;\r\n";
