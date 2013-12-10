#include "MainWindow.h"
#include <QApplication>
#include <QTime>
#include <QDir>

// workaround for a bug on Mavericks
// Finder returns / as the working path of an app bundle
// but if the app is run from terminal, the path is correct
// This method calcluates the path of the bundle from the application's path
QString getCurrentPath()
{
    QDir dir(QApplication::applicationDirPath());
    dir.cdUp();
    dir.cdUp();
    dir.cdUp();
    return dir.absolutePath();
}

int main(int argc, char* argv[])
{
    qsrand(QTime::currentTime().msec());
    QApplication app(argc, argv);

#ifdef Q_OS_OSX
    QDir::setCurrent(getCurrentPath());
#endif

    MainWindow wnd;
    wnd.showMaximized();
    return app.exec();
}


//#include <QTextStream>
//#include <QFile>
//#include <QStringList>

//int main()
//{
//    QMap<QString, int> packages;
//    QFile file("Labeled.csv");
//    if(!file.open(QFile::ReadOnly))
//        return 1;

//    QTextStream is(&file);
//    while(!is.atEnd())
//    {
//        QString line = is.readLine();
//        QStringList sections = line.split(",");
//        if(sections.size() == 2)
//        {
//            QString package = sections[0];
//            int     label   = sections[1].toInt();
//            QMap<QString, int>::iterator it = packages.find(package);

//            if(it == packages.end())
//                it = packages.insert(package, 0);

//            if(label == 1)
//                (*it) ++;
//        }
//    }

//    QFile fileOut("Metric.csv");
//    if(!fileOut.open(QFile::WriteOnly))
//        return 2;

//    QTextStream os(&fileOut);
//    for(QMap<QString, int>::iterator it = packages.begin(); it != packages.end(); ++it)
//        os << it.key() << "," << it.value() << "\r\n";

//    return 0;
//}
