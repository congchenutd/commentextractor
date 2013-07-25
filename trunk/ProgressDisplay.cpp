#include "ProgressDisplay.h"
#include <QProgressBar>
#include <QApplication>

void ProgressBarAdapter::setMaximum(int max) {
    if(_progressBar != 0)
        _progressBar->setMaximum(max);
}

void ProgressBarAdapter::run(const QString&)
{
    if(_progressBar != 0)
        _progressBar->setValue(_progressBar->value() + 1);
    qApp->processEvents();
}
