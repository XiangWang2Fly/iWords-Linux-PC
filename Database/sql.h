#ifndef SQL_H
#define SQL_H
#include <QSqlDatabase>

class SqlServer
{
private:
    SqlServer();
    QSqlDatabase db;

public:
    static SqlServer& GetInstance()
    {
        static SqlServer singleton;
        return singleton;
    }
    bool Init();
    bool Open();
    void Close();
};

#endif // SQL_H
