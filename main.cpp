#include "mainwindow.h"
#include "serverstate.h"
#include <QApplication>
#include <QDesktopWidget>
#include "./Utility/common.h"
#include "./Database/sql.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow mw;
    ServerState ss;
    SqlServer::GetInstance().Init();
    bool serverReady = SqlServer::GetInstance().Open();
    bool showMainWindow = serverReady;

    ss.move((QApplication::desktop()->width() - ss.width())/2, (QApplication::desktop()->height() - ss.height())/2);
    while (showMainWindow == false)
    {
        if (serverReady == true
            ||(serverReady == false && ss.exec() == QDialog::Rejected))
        {
            showMainWindow = true;
            break;
        }
        else
        {
            serverReady = SqlServer::GetInstance().Open();
        }
    }

    mw.Init();
    mw.move((QApplication::desktop()->width() - mw.width())/2, (QApplication::desktop()->height() - mw.height())/2);
    mw.setWindowTitle(Common::version);
    mw.show();
    return a.exec();
}
