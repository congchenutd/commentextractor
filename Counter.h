#ifndef COUNTER_H
#define COUNTER_H

#include "Runnable.h"
#include <QString>

// interface for counter
class Counter
{
public:
    virtual int  getCount() const = 0;
    virtual void increase(int n) = 0;
    virtual void decrease(int n) = 0;
    virtual void reset() = 0;
};

class QLabel;

// shows the count by a label
// implements Counter
class LabelUpdater : public Counter
{
public:
    LabelUpdater(QLabel* label, const QString& labelContent);
    void update();   // update the label with the count

    int getCount() const { return _count; }
    void increase(int n = 1) { _count += n; update(); }
    void decrease(int n = 1) { _count -= n; update(); }
    void reset() { _count = 0; }

private:
    int     _count;
    QLabel* _label;
    QString _labelComment;
};

// Counts number of lines in a file
class LineCounter : public IRunnableOnFile, public LabelUpdater
{
public:
    LineCounter(QLabel* label = 0) : LabelUpdater(label, "LOC = ") {}
    void run(const QString& filePath);
};

// Counts number of files
class FileCounter : public IRunnableOnFile, public LabelUpdater
{
public:
    FileCounter(QLabel* label = 0) : LabelUpdater(label, "NOF = ") {}
    void run(const QString&);
};

// Counts number of tags
class TagCounter : public LabelUpdater
{
public:
    TagCounter(QLabel* label = 0) : LabelUpdater(label, "NOT = ") {}
};

// Counts the lines of tags
class TagLineCounter : public LabelUpdater
{
public:
    TagLineCounter(QLabel* label = 0) : LabelUpdater(label, "LOT = ") {}
};

// Counts packages
class PackageCounter : public LabelUpdater
{
public:
    PackageCounter(QLabel* label = 0) : LabelUpdater(label, "NOP = ") {}
};

// Counts the lines of comments
class CommentLineCounter : public LabelUpdater
{
public:
    CommentLineCounter(QLabel* label = 0) : LabelUpdater(label, "LOM = ") {}
};

// Counts the length of tags
class TagLengthCounter : public LabelUpdater
{
public:
    TagLengthCounter(QLabel* label = 0) : LabelUpdater(label, "WOT = ") {}
};

// Counts the length of comments
class CommentLengthCounter : public LabelUpdater
{
public:
    CommentLengthCounter(QLabel* label = 0) : LabelUpdater(label, "WOM = ") {}
};

#endif // COUNTER_H
