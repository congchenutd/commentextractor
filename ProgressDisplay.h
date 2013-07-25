#ifndef PROGRESSDISPLAY_H
#define PROGRESSDISPLAY_H

#include "Runnable.h"

class QProgressBar;

// Updates a given progress bar
// implements IRunnableOnFile
class ProgressDisplay : public IRunnableOnFile
{
public:
    ProgressDisplay(QProgressBar* progressBar) : _progressBar(progressBar) {}
    void setMaximum(int max);
    void run(const QString&);

private:
    QProgressBar* _progressBar;
};

#endif // PROGRESSDISPLAY_H