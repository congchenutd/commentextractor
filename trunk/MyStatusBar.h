#ifndef MYSTATUSBAR_H
#define MYSTATUSBAR_H

#include <QStatusBar>

class MyStatusBar : public QStatusBar
{
public:
    MyStatusBar(QWidget* parent = 0);

protected:
    void mouseDoubleClickEvent(QMouseEvent*);
};

#endif // MYSTATUSBAR_H
