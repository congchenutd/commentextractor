#include "TextBlock.h"
#include <QTextStream>
#include <QFile>
#include <QRegularExpression>
#include <QFileInfo>

TextBlock::TextBlock(const QString& content, const QString& filePath, int lineNumber)
    : _content(content), _filePath(filePath), _lineNumber(lineNumber)
{}

QString TextBlock::getPackageName() const
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

    return "default";
}

QString TextBlock::getClassName() const
{
    QFile file(_filePath);
    if(file.open(QFile::ReadOnly))
    {
        QTextStream is(&file);
        QString content = is.readAll();

        QRegularExpression re("public\\s+\\w+?class\\s+(?<name>\\w+)");
        QRegularExpressionMatch match = re.match(content);
        if(match.hasMatch())
            return match.captured("name");
    }

    return QFileInfo(_filePath).fileName();
}

QString TextBlock::getCompleteClassName() const {
    return getPackageName() + "." + getClassName();
}
