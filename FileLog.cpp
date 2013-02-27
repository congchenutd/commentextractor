#include "FileLog.h"
#include <QStatusBar>

FileLog::FileLog(QStatusBar* statusBar)
    : _statusBar(statusBar) {}

void FileLog::run(const QString& filePath) {
    _statusBar->showMessage(filePath, 10);
}
