#include "MainWindow.h"
#include <QApplication>
#include <QTime>

int main(int argc, char* argv[])
{
    qsrand(QTime::currentTime().msec());
    QApplication app(argc, argv);
    MainWindow w;
    w.showMaximized();
    return app.exec();
}
