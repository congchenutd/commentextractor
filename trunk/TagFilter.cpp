#include "TagFilter.h"
#include "TagKeywordModel.h"
#include "Extractor.h"
#include "TextBlock.h"
#include <QRegularExpression>
#include <QRegularExpressionMatch>

//////////////////////////////////////////////////////////////////////////////////////
TagFilter::TagFilter(Extractor* extractor, const QString& filter, bool useRegEx)
    : _extractor(extractor),
      _filter(filter),
      _useRegEx(useRegEx)
{}

void TagFilter::run(const QString& filePath)
{
    if(_extractor == 0 || _filter.isEmpty())
        return;

    _result.clear();

    _extractor->run(filePath);               // get all the comments from the file
    QList<TextBlock> allBlocks = _extractor->getResult();

    foreach(const TextBlock& block, allBlocks)    // for each comment
    {
        QString content = block.getContent();
        QStringList tags = _useRegEx ? findTagByRegEx(content)
                                     : findTagByString(content);  // get the tags
        foreach(const QString& tag, tags)
            if(!tag.isEmpty())
                _result << TextBlock(block.getContent(),
                                     block.getFilePath(),
                                     block.getLineNumber(),
                                     tag);
    }
}

QStringList TagFilter::findTagByRegEx(const QString& content) const
{
    QStringList result;
    QRegularExpression rx(_filter, QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch match = rx.match(content);
    while(match.hasMatch())
    {
        QString captured = match.captured("tag").toUpper();  // a capture named "tag"
        QRegularExpressionMatch matchTag = QRegularExpression("\\w+").match(captured);  // find a word
        if(matchTag.hasMatch())
        {
            QString tag = matchTag.captured(0);
            if(!tag.isEmpty())
                result << tag;
        }
        match = rx.match(content, match.capturedStart("tag") + match.capturedLength("tag"));  // next
    }
    return result;
}

QStringList TagFilter::findTagByString(const QString& content) const
{
    QStringList result;
    QStringList filters = _filter.split(";");
    foreach(const QString& filter, filters)
        if(!filter.isEmpty() && content.contains(filter, Qt::CaseInsensitive))
            result << filter;
    return result;
}


//////////////////////////////////////////////////////////////////////////////////////
TagFilterAdapter::TagFilterAdapter(TagFilter* filter, TagKeywordModel* model)
    : _filter(filter),
      _model(model) {}

void TagFilterAdapter::run(const QString& filePath)
{
    _filter->run(filePath);
    QList<TextBlock> tags = _filter->getResult();
    foreach(const TextBlock& tag, tags)
        _model->addTag(tag.getKeyword(), tag);
}

QList<TextBlock> TagFilterAdapter::getResult() const {
    return _filter->getResult();
}


