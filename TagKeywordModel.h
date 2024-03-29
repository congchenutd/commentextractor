#ifndef TAG_KEYWORD_MODEL_H
#define TAG_KEYWORD_MODEL_H

#include <QStandardItemModel>
#include <QHash>

class TagInstanceModel;
class TextBlock;
class TagCounter;
class TagLineCounter;
class TagLengthCounter;

// stores tag keywords and their count
// each tag keyword relates to a TagInstanceModel
class TagKeywordModel : public QStandardItemModel
{
public:
    TagKeywordModel(TagCounter*       counterTag,
                    TagLineCounter*   counterTagLine,
                    TagLengthCounter* counterTagLen,
                    QObject* parent = 0);

    void addTag(const QString& keyword, const TextBlock& block);
    TagInstanceModel* getInstanceModel(const QString& tag) const;
    void clear();
    void pick(int n);          // randomly pick n instances from each tag
    void removeSmall(int n);   // remove tags with less than n instances
    void removeKeyword(const QString& keyword);

    QString getKeyword  (int row) const;   // the keyword in row
    int     getCount    (int row) const;   // the tag count in row
    int     getLineCount(int row) const;   // # lines
    int     getLength   (int row) const;   // # words

    void save(const QString& dirPath);
    void load(const QString& dirPath);
    void exportToFile(const QString& filePath, const QString& modularity);

private:
    int findKeyword(const QString& keyword) const;    // returns the row of the tag
    void removeRow(int row);
    QString getModuleName(const TextBlock& textBlock, const QString& modularity) const;

private:
    typedef QHash<QString, TagInstanceModel*> Keyword2Model;
    Keyword2Model _keyword2Model;
    TagCounter*       _counterTag;
    TagLineCounter*   _counterTagLine;
    TagLengthCounter* _counterTagLen;

public:
    enum {COL_KEYWORD, COL_COUNT, COL_LINECOUNT, COL_LEN};
};


// represent the distribution of tags over the packages
// row is module, col is keyword
class TagDistributionModel : public QStandardItemModel
{
public:
    TagDistributionModel(const QStringList& keywords, QObject* parent = 0);

    void addCount(const QString& moduleName, const QString& keyword);
    void exportToFile(const QString& filePath);

private:
    int findKeywordCol(const QString& keyword)     const;
    int findPackageRow(const QString& packageName) const;

private:
    QHash<QString, int> _package2Row;
    QHash<QString, int> _keyword2Col;
};

#endif // TAG_KEYWORD_MODEL_H
