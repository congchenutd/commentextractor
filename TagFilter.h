#ifndef EXTRACTORDECORATOR_H
#define EXTRACTORDECORATOR_H

#include "Runnable.h"
#include <QMap>
#include <QString>

class Extractor;
class TagKeywordModel;

// Extract tags from a file, and save them in a given model
// uses an extractor to get all the comments
// use a filter (regex or string) to get tags
// regex must have one capture named tag
class TagFilter : public IRunnableOnFile
{
public:
    TagFilter(Extractor* extractor, TagKeywordModel* tagCountModel,
              const QString& filter = QString(), bool useRegEx = true);
    void setFilter(const QString& filter, bool useRegEx) { _filter = filter; _useRegEx = useRegEx; }

    void run(const QString& filePath);

private:
    QStringList findTagByRegEx (const QString& content) const;
    QStringList findTagByString(const QString& content) const;

private:
    Extractor*     _extractor;
    TagKeywordModel* _model;
    QString        _filter;     // regex or substring pattern
    bool           _useRegEx;
};


#endif // EXTRACTORDECORATOR_H
