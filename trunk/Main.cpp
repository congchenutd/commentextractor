#include "MainWindow.h"
#include <QApplication>
#include <QTime>

int main(int argc, char* argv[])
{
    qsrand(QTime::currentTime().msec());
    QApplication app(argc, argv);
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
