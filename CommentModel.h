#ifndef COMMENTMODEL_H
#define COMMENTMODEL_H

#include <QStandardItemModel>

class TextBlock;

class CommentModel : public QStandardItemModel
{
public:
    CommentModel(QObject* parent = 0);

    void clear();
    void addComment(const TextBlock& textBlock);
    TextBlock getComment(int row) const;
    void save(const QString& dirPath);

public:
    enum {COL_PACKAGE, COL_FILE, COL_LINE, COL_COMMENT};
    static QString _fieldSeparator;
    static QString _lineSeparator;
};

#endif // COMMENTMODEL_H
