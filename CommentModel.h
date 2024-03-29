#ifndef COMMENTMODEL_H
#define COMMENTMODEL_H

#include <QStandardItemModel>
#include <QSet>

class TextBlock;
class PackageCounter;
class CommentLineCounter;
class CommentLengthCounter;
class QTextStream;

// Model for comment tab
class CommentModel : public QStandardItemModel
{
public:
    CommentModel(PackageCounter*       counterPkg,
                 CommentLineCounter*   counterCmtLine,
                 CommentLengthCounter* counterCmtLen,
                 QObject* parent = 0);

    void clear();
    void addComment(const TextBlock& textBlock);
    void addComment(const QString& package, const QString& filePath, int lineNum, const QString& content);
    TextBlock getComment(int row) const;

    void save(const QString& dirPath);
    void load(const QString& dirPath);
    void exportToFile(const QString& filePath, int n);   // pick n comments from each package

private:
    void pick(int start, int end, int n, QTextStream& os);

public:
    enum {COL_PACKAGE, COL_FILE, COL_LINE, COL_LEN, COL_COMMENT};
    static QString _fieldSeparator;
    static QString _lineSeparator;

private:
    PackageCounter*       _counterPkg;
    CommentLineCounter*   _counterCmtLine;
    CommentLengthCounter* _counterCmtLen;
    QSet<QString>         _packages;
};

#endif // COMMENTMODEL_H
