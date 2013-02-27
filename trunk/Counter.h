#ifndef COUNTER_H
#define COUNTER_H

#include "Runnable.h"
#include <QString>

class Counter
{
public:
    virtual int getCount() const = 0;
    virtual void increase(int n) = 0;
    virtual void decrease(int n) = 0;
    virtual void reset() = 0;
};

class QLabel;

class LabelUpdater : public Counter
{
public:
    LabelUpdater(QLabel* label, const QString& message);
    void update();
    int getCount() const { return _count; }
    void increase(int n = 1) { _count += n; update(); }
    void decrease(int n = 1) { _count -= n; update(); }
    void reset() { _count = 0; }

private:
    int     _count;
    QString _message;
    QLabel* _label;
};

// Counts number of lines in a file
class LineCounter : public RunnableOnFile, public LabelUpdater
{
public:
    LineCounter(QLabel* label = 0) : LabelUpdater(label, "Line count = ") {}
    void run(const QString& filePath);
};

// Counts number of files
class FileCounter : public RunnableOnFile, public LabelUpdater
{
public:
    FileCounter(QLabel* label = 0) : LabelUpdater(label, "File count = ") {}
    void run(const QString&);
};

// Counts number of tags
class TagCounter : public RunnableOnFile, public LabelUpdater
{
public:
    TagCounter(QLabel* label = 0) : LabelUpdater(label, "Tag count = ") {}
    void run(const QString&) {}  // does not run on a file, but on every tag add/removal
};


#endif // COUNTER_H
