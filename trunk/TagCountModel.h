#ifndef TAGCOUNTMODEL_H
#define TAGCOUNTMODEL_H

#include <QStandardItemModel>
#include <QMap>

class TagDetailModel;
class TextBlock;
class TagCounter;

// stores tag keywords and their count
// each tag keyword relates to a TagDetailModel
class TagCountModel : public QStandardItemModel
{
public:
    TagCountModel(QObject* parent = 0, TagCounter* counter = 0);
    void setCounter(TagCounter* counter) { _counter = counter; }
    void addTag(const QString& tag, const TextBlock& block);
    TagDetailModel* getDetail(const QString& tag) const;
    void clear();
    void pick(int n);          // randomly pick n instances from each tag
    void removeSmall(int n);   // remove tags with less than n instances
    void removeTag(const QString& tag);

    void save(const QString& dirPath, const QString& sourcePath);
    void load(const QString& dirPath);

private:
    int findTag(const QString& tag) const;    // returns the row of the tag
    QString getKeyword(int row) const;        // the keyword in row
    int     getCount  (int row) const;        // the tag count in row
    void removeRow(int row);

private:
    QMap<QString, TagDetailModel*> _details;  // keyword->detail model
    TagCounter*                    _counter;

public:
    enum {COL_TAG, COL_COUNT};
};

#endif // TAGCOUNTMODEL_H
