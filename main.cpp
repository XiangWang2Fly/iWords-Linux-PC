#include "mainwindow.h"
#include "serverstate.h"
#include <QApplication>
#include "./Utility/common.h"
#include "./Database/sql.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SqlServer test;
    MainWindow w;
    ServerState s;
    //if (test.Open())
    if (false)
    {
        w.show();
        w.setWindowTitle(Common::version);
    }
    else
    {

        s.show();
    }


    return a.exec();
}
