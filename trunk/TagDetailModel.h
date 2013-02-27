#ifndef TAGDETAILMODEL_H
#define TAGDETAILMODEL_H

#include <QStandardItemModel>

class TextBlock;

// stores instances of a tag
class TagDetailModel : public QStandardItemModel
{
public:
    TagDetailModel(const QString& tag, QObject* parent = 0);
    TagDetailModel* pick(int n);    // creates a new model containing n random instances
    QString getTag() const { return _tag; }
    void save(const QString& filePath, const QString& sourcePath);

    static QList<TextBlock> fromFile(const QString& filePath, const QString& sourcePath);

private:
    QString _tag;

public:
    enum {COL_FILEPATH, COL_LINE, COL_CONTENT};
};

#endif // TAGDETAILMODEL_H
