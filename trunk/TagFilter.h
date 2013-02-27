#ifndef EXTRACTORDECORATOR_H
#define EXTRACTORDECORATOR_H

#include "Runnable.h"
#include <QMap>
#include <QString>

class Extractor;
class TagCountModel;

// Extract content of a file, filter it by regex, and save it in a given model
class TagFilter : public RunnableOnFile
{
public:
    TagFilter(Extractor* extractor, TagCountModel* tagCountModel,
              const QString& filter = QString(), bool useRegEx = true);
    void setFilter(const QString& filter, bool useRegEx) { _filter = filter; _useRegEx = useRegEx; }

    void run(const QString& filePath);

private:
    QStringList findTagByRegEx (const QString& content) const;
    QStringList findTagByString(const QString& content) const;

private:
    Extractor*     _extractor;
    TagCountModel* _model;
    QString        _filter;
    bool           _useRegEx;
};


#endif // EXTRACTORDECORATOR_H
