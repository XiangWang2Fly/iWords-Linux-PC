#include "mainwindow.h"
#include <QApplication>
#include "common.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.setWindowTitle(Common::version);
    return a.exec();
}
