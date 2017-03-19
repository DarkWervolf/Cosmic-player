#include <QApplication>
#include "mainwindow.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow mw;
    mw.setMinimumHeight(325);
    mw.setMaximumHeight(325);
    mw.setMinimumWidth(374);
    mw.setMaximumWidth(374);
    mw.show();
    return a.exec();
}
