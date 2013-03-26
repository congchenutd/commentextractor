#include "TextBlock.h"
#include <QTextStream>
#include <QDebug>

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
        while(!is.atEnd())
        {
            QString line = is.readLine();
            if(line.startsWith("package"))
                result = line.remove("package").simplified();
        }
    }
    return result;
}
