#include "sql.h"
#include <QtSql>

SqlServer::SqlServer()
{

}



bool SqlServer::Init()
{
    this->db = QSqlDatabase::addDatabase("QODBC");
    this->db.setHostName("139.196.52.48");
    this->db.setDatabaseName("iWordsDB");
    this->db.setUserName("sa");
    this->db.setPassword("521021");

    if (!this->db.isValid())
    {
        qDebug("%s", qPrintable(this->db.lastError().text()));
        return true;
    }
    else
    {
        return false;
    }
}

bool SqlServer::Open()
{
    if (this->db.open())
    {
        return true;
    }
    else
    {
        return false;
    }
}

