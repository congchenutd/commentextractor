#include "Counter.h"
#include <QFile>
#include <QTextDocument>
#include <QLabel>

LabelUpdater::LabelUpdater(QLabel* label, const QString& message)
    : _count(0), _label(label), _message(message)
{}

void LabelUpdater::update() {
    if(_label != 0)
        _label->setText(QObject::tr("%1%2 ").arg(_message).arg(_count));
}

void LineCounter::run(const QString& filePath)
{
    QFile file(filePath);
    if(file.open(QFile::ReadOnly))
        increase(QTextDocument(file.readAll()).lineCount());  // increase the count of lines
}

void FileCounter::run(const QString &) {
    increase(1);  // increase the count of files by 1
}


