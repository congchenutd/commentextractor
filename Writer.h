#ifndef WRITER_H
#define WRITER_H

#include <QString>

class TextBlock;

// Write text blocks to file
class IWriter
{
public:
    virtual void write(const QString& filePath, const QList<TextBlock>& blocks) const = 0;
};

class CustomWriter : public IWriter
{
public:
    CustomWriter(const QString& separator) : _separator(separator) {}
    void write(const QString& filePath, const QList<TextBlock>& blocks) const;

private:
    QString _separator;
};

class CSVWriter : public IWriter
{
public:
    CSVWriter();
    void write(const QString& filePath, const QList<TextBlock>& blocks) const;

private:
    CustomWriter _customWriter;
};


#endif // WRITER_H
