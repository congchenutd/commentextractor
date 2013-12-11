#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include "Runnable.h"
#include "TextBlock.h"
#include <QList>
#include <QString>

class QString;
class CommentModel;

// Extracts substrings from a file
// the substrings are caught by a regular expression
// implements IRunnableOnFile
class Extractor : public IRunnableOnFile
{
public:
    Extractor(const QString& pattern);
    QList<TextBlock> getResult() const { return _result; }

    void run(const QString& filePath);  // run extractor on the file

private:
    // extract one matching substring from text, starting from cursor,
    // filePath is the file containing the text
    // cursor will be updated after each call
    TextBlock extractOne(const QString& text, const QString& filePath, int& cursor) const;

    // returns the line num of the cursor in the text
    int getLineNumber(const QString& text, int cursor) const;

private:
    QString          _pattern;          // RegEx pattern
    QList<TextBlock> _result;
};

// connecting an Extractor to a CommentModel
class ExtractorAdapter : public IRunnableOnFile
{
public:
    ExtractorAdapter(Extractor* extractor, CommentModel* model = 0);
    QList<TextBlock> getResult() const;

    void run(const QString& filePath);

private:
    Extractor*    _extractor;
    CommentModel* _model;
};

#endif // EXTRACTOR_H
