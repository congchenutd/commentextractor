#include "ProgressDisplay.h"
#include <QProgressBar>
#include <QApplication>

void ProgressDisplay::setMaximum(int max) {
    if(_progressBar != 0)
        _progressBar->setMaximum(max);
}

void ProgressDisplay::run(const QString&)
{
    _progressBar->setValue(_progressBar->value() + 1);
    qApp->processEvents();
}
