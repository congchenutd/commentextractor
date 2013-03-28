#ifndef TAGCOUNTMODEL_H
#define TAGCOUNTMODEL_H

#include <QStandardItemModel>
#include <QHash>

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
    void exportToFile(const QString& filePath);

private:
    int findTag(const QString& tag) const;    // returns the row of the tag
    void remove(int row);

private:
    typedef QHash<QString, TagInstanceModel*> Keyword2Model;
    Keyword2Model _keyword2Model;
    TagCounter* _counter;
    QString     _projectPath;

public:
    enum {COL_TAG, COL_COUNT};
};


// represent the distribution of tags over the packages
// row is package, col is keyword
class TagDistributionModel : public QStandardItemModel
{
public:
    TagDistributionModel(const QStringList& keywords, QObject* parent = 0);

    void addCount(const QString& packageName, const QString& keyword);
    void exportToFile(const QString& filePath);

private:
    int findKeywordCol(const QString& keyword)     const;
    int findPackageRow(const QString& packageName) const;

private:
    QHash<QString, int> _package2Row;
    QHash<QString, int> _keyword2Col;

};

#endif // TAGCOUNTMODEL_H
