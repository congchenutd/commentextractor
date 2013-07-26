#ifndef TAG_INSTANCE_MODEL_H
#define TAG_INSTANCE_MODEL_H

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
    QString getContent (int row) const;
    QString getFilePath(int row) const;
    int     getLineNum (int row) const;

    TextBlock        getTextBlock(int row) const;
    QList<TextBlock> getAllTextBlocks()    const;
    void setTextBlock(int row, const TextBlock& textBlock);
    void addTextBlock(const TextBlock& textBlock);

    // load a keyword's instances from file
    static QList<TextBlock> load(const QString& filePath, const QString& projectPath);

    // save a keyword's instances to file
    void save(const QString& filePath);

private:
    QString _keyword;

public:
    enum {COL_FILEPATH, COL_LINENUM, COL_CONTENT};
    static QString _lineSeparator;
};

#endif // TAG_INSTANCE_MODEL_H
