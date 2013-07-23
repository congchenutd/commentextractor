#include "CommentModel.h"
#include "TextBlock.h"
#include "Settings.h"
#include <QTextStream>
#include <QDir>
#include <QRegularExpression>

CommentModel::CommentModel(QObject* parent)
    : QStandardItemModel(parent)
{
    setColumnCount(4);
    setHeaderData(COL_PACKAGE, Qt::Horizontal, tr("Package"));
    setHeaderData(COL_FILE,    Qt::Horizontal, tr("File"));
    setHeaderData(COL_LINE,    Qt::Horizontal, tr("Line"));
    setHeaderData(COL_COMMENT, Qt::Horizontal, tr("Comment"));
}

void CommentModel::clear() {
    removeRows(0, rowCount());
}

void CommentModel::addComment(const TextBlock& textBlock)
{
    QString content = textBlock.getContent().remove(QRegularExpression("[\\*\\/]")).simplified();
    if(content.isEmpty())
        return;
    int lastRow = rowCount();
    insertRow(lastRow);
    setData(index(lastRow, COL_PACKAGE), textBlock.getPackageName());
    setData(index(lastRow, COL_FILE),    textBlock.getFilePath());
    setData(index(lastRow, COL_LINE),    textBlock.getLineNumber());
    setData(index(lastRow, COL_COMMENT), content);
}

TextBlock CommentModel::getComment(int row) const {
    return TextBlock(data(index(row, COL_COMMENT)).toString(),
                     data(index(row, COL_FILE)).toString(),
                     data(index(row, COL_LINE)).toInt());
}

void CommentModel::save(const QString& dirPath)
{
    QFile file(dirPath + QDir::separator() + "Comments.csv");
    if(file.open(QFile::WriteOnly))
    {
        QTextStream os(&file);
        os << Settings().getProjectPath() << _lineSeparator;

        for(int row = 0; row < rowCount(); ++row)
        {
            TextBlock comment = getComment(row);
            QString filePath = comment.getFilePath();                           // absolute path
            filePath.remove(Settings().getProjectPath(), Qt::CaseInsensitive);  // to relative path

            os << row + 1                  << _fieldSeparator
               << comment.getPackageName() << _fieldSeparator
               << filePath                 << _fieldSeparator
               << comment.getLineNumber()  << _fieldSeparator
               << comment.getContent()     << _lineSeparator;
        }
    }
}

QString CommentModel::_fieldSeparator = "|";
QString CommentModel::_lineSeparator = "\r\n";
