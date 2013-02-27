#include "Extractor.h"
#include "TextBlock.h"
#include <QRegExp>
#include <QFile>

Extractor::Extractor(const QString& pattern)
    : _pattern(pattern) {}

void Extractor::run(const QString& filePath)
{
    _result.clear();            // once a file

    QFile file(filePath);
    if(!file.open(QFile::ReadOnly))
        return;
    QString text = file.readAll();

    int cursor = 0;
    do {
        TextBlock oneMatch = extractOne(text, filePath, cursor);
        if(!oneMatch.getContent().isEmpty())
            _result << oneMatch;
    } while(cursor > -1);
}

int Extractor::getLineNumber(const QString& text, int cursor) const {
    return text.left(cursor).count("\n") + 1;
}

TextBlock Extractor::extractOne(const QString& text, const QString& filePath, int& cursor) const
{
    QRegExp rx(_pattern);
    int index = rx.indexIn(text, cursor);
    if(index == -1)
    {
        cursor = -1;
        return TextBlock();
    }

    cursor = index + rx.matchedLength();
    int lineNumber = getLineNumber(text, index);
    return TextBlock(text.mid(index, rx.matchedLength()), filePath, lineNumber);
}
