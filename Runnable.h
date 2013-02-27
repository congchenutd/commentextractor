#ifndef RUNNABLE_H
#define RUNNABLE_H

#include <QList>

class QString;

// Interface for actions on a file (actors)
class IRunnableOnFile
{
public:
    virtual void run(const QString& filePath) = 0;
};

typedef QList<IRunnableOnFile*> Actors;

#endif // RUNNABLE_H
