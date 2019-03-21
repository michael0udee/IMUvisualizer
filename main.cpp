#include "mainwindow.h"
#include <QApplication>

// www.amin-ahmadi.com

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
