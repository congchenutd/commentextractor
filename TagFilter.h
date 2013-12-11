#ifndef EXTRACTORDECORATOR_H
#define EXTRACTORDECORATOR_H

#include "Runnable.h"
#include <QList>
#include <QString>

class Extractor;
class TagKeywordModel;
class TextBlock;

// Extract tags from a file, and save them in a given model
// uses an extractor to get all the comments
// use a filter (regex or string) to get tags
// regex must have one capture named tag
class TagFilter : public IRunnableOnFile
{
public:
    TagFilter(Extractor* extractor, const QString& filter, bool useRegEx);
    QList<TextBlock> getResult() const { return _result; }

    void run(const QString& filePath);

private:
    QStringList findTagByRegEx (const QString& content) const;
    QStringList findTagByString(const QString& content) const;

private:
    Extractor*       _extractor;
    QString          _filter;     // regex or substring pattern
    bool             _useRegEx;
    QList<TextBlock> _result;
};

// connecting a TagFilter to a TagKeywordModel
class TagFilterAdapter : public IRunnableOnFile
{
public:
    TagFilterAdapter(TagFilter* filter, TagKeywordModel* model);
    void run(const QString& filePath);
    QList<TextBlock> getResult() const;

private:
    TagFilter*       _filter;
    TagKeywordModel* _model;
};

#endif // EXTRACTORDECORATOR_H
