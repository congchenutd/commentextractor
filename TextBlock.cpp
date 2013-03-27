#include "TextBlock.h"
#include <QTextStream>
#include <QFile>
#include <QRegularExpression>

TextBlock::TextBlock(const QString& content, const QString& filePath, int lineNumber)
    : _content(content), _lineNumber(lineNumber)
{
    setFilePath(filePath);
}

void TextBlock::setFilePath(const QString& filePath)
{
    _filePath = filePath;
    _package  = getPackage(filePath);
}

QString TextBlock::getPackage(const QString& filePath) const
{
    QString result;
    QFile file(filePath);
    if(file.open(QFile::ReadOnly))
    {
        QTextStream is(&file);
        QString content = is.readAll();

        QRegularExpression re("package\\s+([\\w|\\.])+");
        QRegularExpressionMatch match = re.match(content);
        if(match.hasMatch())
            result = match.captured(0);
    }

    return filePath;
}
