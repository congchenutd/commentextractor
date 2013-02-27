#ifndef FILELOG_H
#define FILELOG_H

#include "Runnable.h"

class QStatusBar;

// shows file path of current file on given status bar
class FileLog : public IRunnableOnFile
{
public:
    FileLog(QStatusBar* statusBar);
    void run(const QString& filePath);

private:
    QStatusBar* _statusBar;
};

#endif // FILELOG_H
