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
    bool showMainWindow = false;
    //if (test.Open())
    while (false)
    {
        s.show();
        if(s.accepted)
        {

            break;

        }
        else
        {

        }
    }
    else
    {


    }


    w.show();
    w.setWindowTitle(Common::version);


    return a.exec();
}
