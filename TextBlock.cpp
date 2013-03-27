#include "TextBlock.h"
#include <QTextStream>
#include <QFile>
#include <QRegularExpression>

TextBlock::TextBlock(const QString& content, const QString& filePath, int lineNumber)
    : _content(content), _filePath(filePath), _lineNumber(lineNumber)
{}

QString TextBlock::getPackage() const
{
    QFile file(_filePath);
    if(file.open(QFile::ReadOnly))
    {
        QTextStream is(&file);
        QString content = is.readAll();

        QRegularExpression re("package\\s+(?<name>[\\w|\\.]+);");
        QRegularExpressionMatch match = re.match(content);
        if(match.hasMatch())
            return match.captured("name");
    }

    return _filePath;
}
