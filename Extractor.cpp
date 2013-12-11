#include "Extractor.h"
#include "TextBlock.h"
#include "CommentModel.h"
#include <QRegExp>
#include <QFile>

Extractor::Extractor(const QString& pattern)
    : _pattern(pattern) {}

void Extractor::run(const QString& filePath)
{
    _result.clear();            // reset result for each run

    // read file
    QFile file(filePath);
    if(!file.open(QFile::ReadOnly))
        return;
    QString text = file.readAll();

    // find all matches
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

////////////////////////////////////////////////////////////////////////////////////
ExtractorAdapter::ExtractorAdapter(Extractor* extractor, CommentModel* model)
    : _extractor(extractor),
      _model(model) {}

QList<TextBlock> ExtractorAdapter::getResult() const {
    return _extractor->getResult();
}

void ExtractorAdapter::run(const QString& filePath)
{
    _extractor->run(filePath);
    if(_model != 0)
    {
        QList<TextBlock> result = getResult();
        foreach(const TextBlock& block, result)
            _model->addComment(block);
    }
}
