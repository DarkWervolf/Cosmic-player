#include <QApplication>
#include "mainwindow.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow mw;
    mw.setMinimumHeight(160);
    mw.setMaximumHeight(160);
    mw.setMinimumWidth(238);
    mw.setMaximumWidth(238);
    mw.show();
    return a.exec();
}
