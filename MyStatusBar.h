#ifndef MYSTATUSBAR_H
#define MYSTATUSBAR_H

#include <QStatusBar>

// double click saves message to clipboard
class MyStatusBar : public QStatusBar
{
public:
    MyStatusBar(QWidget* parent = 0);

protected:
    void mouseDoubleClickEvent(QMouseEvent*);
};

#endif // MYSTATUSBAR_H
