#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include "Runnable.h"
#include "TextBlock.h"
#include <QList>
#include <QString>

class QString;

// Extractor extracts substrings from a file
// the substrings are caught by a regular expression
// implements RunnableOnFile
class Extractor : public RunnableOnFile
{
public:
    void setPattern(const QString& pattern) { _pattern = pattern; }  // Regular expression pattern
    QList<TextBlock> getResult() const { return _result; }

    void run(const QString& filePath);

private:
    TextBlock extractOne   (const QString& text, const QString& filePath, int& cursor) const;
    int       getLineNumber(const QString& text, int cursor) const;

private:
    QString _pattern;
    QList<TextBlock> _result;
};


#endif // EXTRACTOR_H
