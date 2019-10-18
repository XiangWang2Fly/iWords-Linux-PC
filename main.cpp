#include "mainwindow.h"
#include <QApplication>
#include "./Utility/common.h"
#include "./Database/sql.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.setWindowTitle(Common::version);
    SqlServer test;
    test.Open();

    return a.exec();
}
