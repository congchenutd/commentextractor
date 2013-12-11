#include "CommentModel.h"
#include "TextBlock.h"
#include "Settings.h"
#include "Counter.h"
#include <QTextStream>
#include <QDir>
#include <QRegularExpression>
#include <QApplication>

CommentModel::CommentModel(PackageCounter* packageCounter,
                           CommentLineCounter* lineCounter,
                           QObject* parent)
    : QStandardItemModel(parent),
      _packageCounter(packageCounter),
      _lineCounter(lineCounter)
{
    setColumnCount(4);
    setHeaderData(COL_PACKAGE, Qt::Horizontal, tr("Package"));
    setHeaderData(COL_FILE,    Qt::Horizontal, tr("File"));
    setHeaderData(COL_LINE,    Qt::Horizontal, tr("Line"));
    setHeaderData(COL_COMMENT, Qt::Horizontal, tr("Comment"));
}

void CommentModel::clear()
{
    removeRows(0, rowCount());
    _packageCounter->reset();
    _lineCounter   ->reset();
    _packages.clear();
}

void CommentModel::addComment(const TextBlock& textBlock)
{
    QString content = textBlock.getContent().remove(QRegularExpression("[\\*\\/]")).simplified();
    addComment(textBlock.getPackageName(), textBlock.getFilePath(), textBlock.getLineNumber(), content);
}

void CommentModel::addComment(const QString& package, const QString& filePath,
                              int lineNum, const QString& content)
{
    if(content.isEmpty())
        return;

    int lastRow = rowCount();
    insertRow(lastRow);
    setData(index(lastRow, COL_PACKAGE), package);
    setData(index(lastRow, COL_FILE),    filePath);
    setData(index(lastRow, COL_LINE),    lineNum);
    setData(index(lastRow, COL_COMMENT), content);

    // count package
    if(!_packages.contains(package))  // new package
    {
        _packages.insert(package);
        _packageCounter->increase();
    }

    _lineCounter->increase(content.count("\r\n") + 1);
}

TextBlock CommentModel::getComment(int row) const {
    return TextBlock(data(index(row, COL_COMMENT)).toString(),
                     data(index(row, COL_FILE))   .toString(),
                     data(index(row, COL_LINE))   .toInt());
}

void CommentModel::save(const QString& dirPath)
{
    QFile file(dirPath + QDir::separator() + "Comments.csv");
    if(file.open(QFile::WriteOnly))
    {
        QTextStream os(&file);
        os << Settings().getProjectPath() << _lineSeparator;                    // proj path

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

void CommentModel::load(const QString& dirPath)
{
    clear();

    QFile file(dirPath + QDir::separator() + "Comments.csv");
    if(file.open(QFile::ReadOnly))
    {
        QTextStream is(&file);
        QString projectPath = is.readLine();
        Settings().setProjectPath(projectPath);       // proj path

        while(!is.atEnd())
        {
            QStringList sections = is.readLine().split(_fieldSeparator);
            if(sections.size() != 5)   // column 0 is id, ignore
                continue;

            QString filePath = projectPath + sections[2];   // relative -> absolute path
            addComment(sections[1], filePath, sections[3].toInt(), sections[4]);
        }
    }
}

void CommentModel::exportToFile(const QString& filePath, int n)
{
    QFile file(filePath);
    if(!file.open(QFile::WriteOnly))
        return;
    QTextStream os(&file);

    sort(COL_PACKAGE);

    int start = 0;       // start row of current package
    int end   = 0;       // end points to the start of next package
    QString currentPackage = data(index(end++, COL_PACKAGE)).toString();
    do
    {
        QString nextPackage = data(index(end, COL_PACKAGE)).toString();
        if(nextPackage != currentPackage)  // a new package
        {
            pick(start, end, n, os);       // pick n rows from previous package
            currentPackage = nextPackage;  // start the new package
            start = end;
        }
        end ++;
    } while(end < rowCount());

    pick(start, end, n, os);               // last package
}

void CommentModel::pick(int start, int end, int n, QTextStream& os)
{
    if(start > end)
        return;

    if(end - start <= n)   // pick all
    {
        for(int row = start; row < end; ++row)
            os << data(index(row, COL_PACKAGE)).toString() << _fieldSeparator
               << data(index(row, COL_FILE))   .toString() << _fieldSeparator
               << data(index(row, COL_LINE))   .toString() << _fieldSeparator
               << data(index(row, COL_COMMENT)).toString() << _lineSeparator;
    }
    else                   // pick n rows
    {
        QSet<int> rows;
        for(int i = 0; i < n; ++i)
        {
            int row;
            do {
                row = qrand() % (end - start) + start;
            } while(rows.contains(row));
            rows << row;
        }

        foreach(int row, rows)
            os << data(index(row, COL_PACKAGE)).toString() << _fieldSeparator
               << data(index(row, COL_FILE))   .toString() << _fieldSeparator
               << data(index(row, COL_LINE))   .toString() << _fieldSeparator
               << data(index(row, COL_COMMENT)).toString() << _lineSeparator;
    }
}

QString CommentModel::_fieldSeparator = "|";
QString CommentModel::_lineSeparator = "\r\n";
