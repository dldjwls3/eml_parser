#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>

class database
{
private:
    QSqlDatabase db;
public:
    database();
    void insert_row(QString row);
    void close();
};

#endif // DATABASE_H
