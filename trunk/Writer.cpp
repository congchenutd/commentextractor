#include "Writer.h"
#include "TextBlock.h"
#include <QFile>
#include <QTextStream>

void CustomWriter::write(const QString& filePath, const QList<TextBlock>& blocks) const
{
    QFile file(filePath);
    if(!file.open(QFile::WriteOnly))
        return;

    QTextStream os(&file);
    os << "#,File path,Line number,Content\r\n";

    int i = 0;
    foreach(const TextBlock& block, blocks)
    {
        os << ++i << _separator
           << "\"" << block.getFilePath() << "\"" << _separator
           << block.getLineNumber() << ","
           << "\"" << block.getContent()  << "\"\r\n";
    }
}

CSVWriter::CSVWriter()
    : _customWriter(",") {}

void CSVWriter::write(const QString& filePath, const QList<TextBlock>& blocks) const {
    _customWriter.write(filePath, blocks);
}
