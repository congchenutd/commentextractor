#ifndef TAGDETAILMODEL_H
#define TAGDETAILMODEL_H

#include <QStandardItemModel>

class TextBlock;

// stores instances of a tag
class TagInstanceModel : public QStandardItemModel
{
public:
    TagInstanceModel(const QString& tag, QObject* parent = 0);
    TagInstanceModel* pick(int n);    // creates a new model containing n random instances
    QString getKeyword() const { return _keyword; }

    // for convenience
    QString getFilePath(int row) const;
    int     getLineNum (int row) const;
    QString getContent (int row) const;

    static QList<TextBlock> load(const QString& filePath, const QString& projectPath);
    void save(const QString& filePath, const QString& projectPath);

private:
    QString _keyword;

public:
    enum {COL_FILEPATH, COL_LINENUM, COL_CONTENT};
    static QString _lineSeparator;
};

#endif // TAGDETAILMODEL_H
