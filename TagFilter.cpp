#include "TagFilter.h"
#include "TagCountModel.h"
#include "Extractor.h"
#include <QRegularExpression>
#include <QRegularExpressionMatch>

//////////////////////////////////////////////////////////////////////////////////////
TagFilter::TagFilter(Extractor* extractor, TagCountModel* tagCountModel,
                     const QString& filter, bool useRegEx)
    : _extractor(extractor),
      _model(tagCountModel),
      _filter(filter),
      _useRegEx(useRegEx)
{}

void TagFilter::run(const QString& filePath)
{
    if(_extractor == 0 || _filter.isEmpty())
        return;

    _extractor->run(filePath);
    QList<TextBlock> allBlocks = _extractor->getResult();   // all comments

    foreach(const TextBlock& block, allBlocks)
    {
        QString content = block.getContent();
        QStringList tags = _useRegEx ? findTagByRegEx(content) : findTagByString(content);
        foreach(const QString& tag, tags)
            if(!tag.isEmpty())
                _model->addTag(tag, block);
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
        match = rx.match(content, match.capturedStart("tag") + match.capturedLength("tag"));
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
