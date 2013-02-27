#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include "Runnable.h"
#include "TextBlock.h"
#include <QList>
#include <QString>

class QString;

// Extractor extracts substrings from a file
// the substrings are caught by a regular expression
// implements IRunnableOnFile
class Extractor : public IRunnableOnFile
{
public:
    Extractor(const QString& pattern);   // RegEx pattern
    QList<TextBlock> getResult() const { return _result; }

    void run(const QString& filePath);  // run extractor on the file with filePath

private:
    // extract one matched substring from text, starting from cursor, filePath is the file containing the text
    // cursor will be updated after each call
    TextBlock extractOne(const QString& text, const QString& filePath, int& cursor) const;

    // returns the line num of the cursor in the text
    int getLineNumber(const QString& text, int cursor) const;

private:
    QString _pattern;
    QList<TextBlock> _result;
};


#endif // EXTRACTOR_H
