#ifndef TAGCOUNTMODEL_H
#define TAGCOUNTMODEL_H

#include <QStandardItemModel>
#include <QMap>

class TagInstanceModel;
class TextBlock;
class TagCounter;

// stores tag keywords and their count
// each tag keyword relates to a TagDetailModel
class TagCountModel : public QStandardItemModel
{
public:
    TagCountModel(QObject* parent = 0, TagCounter* counter = 0);

    QString getProjectPath() const { return _projectPath; }
    void setProjectPath(const QString& projectPath);

    void addTag(const QString& tag, const TextBlock& block);
    TagInstanceModel* getInstanceModel(const QString& tag) const;
    void clear();
    void pick(int n);          // randomly pick n instances from each tag
    void removeSmall(int n);   // remove tags with less than n instances
    void remove(const QString& tag);

    QString getKeyword(int row) const;        // the keyword in row
    int     getCount  (int row) const;        // the tag count in row

    void save(const QString& dirPath);
    void load(const QString& dirPath);

private:
    int findTag(const QString& tag) const;    // returns the row of the tag
    void remove(int row);

private:
    QMap<QString, TagInstanceModel*> _instanceModels;  // keyword->instance model
    TagCounter* _counter;
    QString     _projectPath;

public:
    enum {COL_TAG, COL_COUNT};
};

#endif // TAGCOUNTMODEL_H
