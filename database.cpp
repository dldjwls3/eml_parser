#include "database.h"

#include <QString>
#include <QDebug>


database::database()
{

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("emlfiles_database.db");
    if(!db.open()) {
        qDebug() << "Fail to connect" << endl;
    }

    QString q_str = "create table if not exists EMLFILES "
                    "(id integer primary key, "
                    "composed varchar(40), "
                    "from_text varchar(30), "
                    "from_email varchar(50), "
                    "to_text varchar(50), "
                    "to_email varchar(50), "
                    "subject varchar(50), "
                    "receive_path varchar(50), "
                    "attachment integer, "
                    "md5_hash varchar(50));";
    QSqlQuery query;
    if(!query.prepare(q_str)) {
        qDebug() << "Fail to prepare query" << endl;
    }
    if(!query.exec(q_str)) {
        qDebug() << "Fail to create table" << endl;
    }

//    tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("compoesd date"));
//    tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("from text"));
//    tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("from email"));
//    tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("to text"));
//    tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem("to email"));
//    tableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem("subject"));
//    tableWidget->setHorizontalHeaderItem(6, new QTableWidgetItem("receive path"));
//    tableWidget->setHorizontalHeaderItem(7, new QTableWidgetItem("content"));
//    tableWidget->setHorizontalHeaderItem(8, new QTableWidgetItem("attachment"));
//    tableWidget->setHorizontalHeaderItem(9, new QTableWidgetItem("md5 hash"));
}

void database::insert_row(QString values) {


}

void database::close() {
    db.close();
}
