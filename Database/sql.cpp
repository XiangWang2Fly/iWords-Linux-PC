#include "sql.h"
#include <QSqlDatabase>
#include <QtSql>

SqlServer::SqlServer()
{

}

bool SqlServer::Open()
{
    QStringList drivers = QSqlDatabase::drivers();
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setHostName("139.196.52.48");

    db.setDatabaseName("iWordsDB");
    db.setUserName("sa");
    db.setPassword("521021");

    if (!db.isValid())
    {
        qDebug("%s", qPrintable(db.lastError().text()));
        return true;
    }

    if (db.open())
    {
        return true;
    }
    else
    {
        QString test = db.lastError().text();
        qDebug("%s", qPrintable(test));
        return false;
    }

}
