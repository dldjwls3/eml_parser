#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include "eml.h"
#include "regex_set.h"
#include "content_parser.h"

#include "mainwindow.h"
#include <QtWidgets>
#include <QDebug>
#include <list>


string csv_string_format(string input){

    string ret = "";
    for(int i = 0; i < input.length(); i++) {
        if(input[i] == ',' || input[i] == '\n')
            continue;
        ret += input[i];
    }

    return ret;
}

QString double_apostrophe(QString in) {
    QString ret;
    for(int i = 0; i < in.length(); i++) {
        if(in[i] == '\'')
            ret += '\'';
        ret += in[i];
    }
    return ret;
}

void MainWindow::createActions(){
    QMenu *fileMenu = menuBar()->addMenu("&File");

    QAction *openAct = new QAction("&Open", this);
    openAct->setShortcut(QKeySequence::Open);
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    fileMenu->addAction(openAct);

    QAction *exportCSVAct = new QAction("&Export", this);
    connect(exportCSVAct, &QAction::triggered, this, &MainWindow::exportCSV);
    fileMenu->addAction(exportCSVAct);


}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    db = new database();
    tableWidget = new QTableWidget(this);
    tableWidget->setSortingEnabled(true);
    setCentralWidget(tableWidget);
    //setAcceptDrops(true);
    createActions();
    connect(tableWidget, &QTableWidget::itemDoubleClicked, this, &MainWindow::click_view);

    this->resize(1000, 900);
}


void MainWindow::open()
{

     QStringList filenames = QFileDialog::getOpenFileNames(this, "EML FILE",
                                                           R"(C:\Users\LSJ\Documents\Qt\eml_parser-master\eml sample)",
                                                           "eml files (*.eml);;All files (*.*)" );
     int row = filenames.size();

     contents = (content_parser**)malloc(sizeof(content_parser*) * row);
     metas = (eml_metadata**)malloc(sizeof(eml_metadata*) * row);
     if(contents == NULL){
         qDebug() << "MainWindow's contents NULLptr" << endl;
         return;
     }
     if(metas == NULL){
         qDebug() << "MainWindow's metas NULLptr" << endl;
         return;
     }


     tableWidget->setColumnCount(10);
     tableWidget->setRowCount(row);


     tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("composed date"));
     tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("from text"));
     tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("from email"));
     tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("to text"));
     tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem("to email"));
     tableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem("subject"));     
     tableWidget->setHorizontalHeaderItem(6, new QTableWidgetItem("receive path"));
     tableWidget->setHorizontalHeaderItem(7, new QTableWidgetItem("content"));
     tableWidget->setHorizontalHeaderItem(8, new QTableWidgetItem("attachment"));
     tableWidget->setHorizontalHeaderItem(9, new QTableWidgetItem("md5 hash"));



     for(int i = 0; i < row; i++) {
         QString fileName = filenames.at(i);
         std::string path = fileName.toLocal8Bit().constData();
         ifstream file;
         string key, value, str;
         eml eml_file;


         file.open(path, ios::in);
         if(file.fail()){
             qDebug() << "EML FILE OPEN FAILED" << endl;
             continue;
         }

         try {
             eml_file.eml_parse(file);
         }
         catch(int e) {
             qDebug() << e << ":" << fileName << endl;
         }

         contents[i] = new content_parser(eml_file.content);
         metas[i] = new eml_metadata(eml_file.meta);

         file.close();

         QString row;
         QString composed_date = QString::fromLocal8Bit(metas[i]->composed_date.c_str());
         QString from_text = QString::fromLocal8Bit(metas[i]->from_text.c_str());
         QString from_email = QString::fromLocal8Bit(metas[i]->from_email.c_str());
         QString to_text = QString::fromLocal8Bit(metas[i]->to_text.c_str());
         QString to_email = QString::fromLocal8Bit(metas[i]->to_email.c_str());
         QString subject = QString::fromLocal8Bit(metas[i]->subject.c_str());
         QString received_path;
         //QString content;
         QString attachment = QString::number(eml_file.content.attachments.size());
         QString md5_hash;

         QTableWidgetItem *newItem[10];
         newItem[0] = new QTableWidgetItem(composed_date);
         newItem[1] = new QTableWidgetItem(from_text);
         newItem[2] = new QTableWidgetItem(from_email);
         newItem[3] = new QTableWidgetItem(to_text);
         newItem[4] = new QTableWidgetItem(to_email);
         newItem[5] = new QTableWidgetItem(subject);

         string temp = "";
         for(vector<received>::iterator iter = metas[i]->receive_list.begin(); iter != metas[i]->receive_list.end(); iter++){
             string by = iter->by[ip], from = iter->from[ip];
             if(by == "") {
                 by = iter->by[word];
             }
             if(from == "") {
                 from = iter->from[word];
             }
             temp += "(" + by + " <- " + from + ") ";
         }
         received_path = QString::fromLocal8Bit(temp.c_str());
         newItem[6] = new QTableWidgetItem(received_path);

         newItem[7] = new QTableWidgetItem(QString::fromLocal8Bit((eml_file.content.content.substr(0, 30) + "...").c_str()));
         newItem[8] = new QTableWidgetItem(attachment);

         for(list<struct file_container>::iterator iter = eml_file.content.attachments.begin(); iter != eml_file.content.attachments.end(); iter++){
             md5_hash += QString::fromLocal8Bit(iter->id_or_name.c_str()) + ": " + QString::fromLocal8Bit(iter->md5_hash.c_str()) + "\n";
         }
         newItem[9] = new QTableWidgetItem(md5_hash);


//         from_text += double_apostrophe(from_text);
//         from_email += double_apostrophe(from_email);
//         to_text += double_apostrophe(to_text);
//         to_email += double_apostrophe(to_email);
//         subject += double_apostrophe(subject);
//         received_path += double_apostrophe(received_path);
//         md5_hash += double_apostrophe(md5_hash);

         row = "'" + composed_date + "', " + "'" + from_text + "', " + "'" + from_email + "', " + "'" + to_text + "', " + "'" + to_email + "', " +
                 "'" + "subject" + "', " + "'" + received_path + "', " + attachment + ", " + "'" + md5_hash + "'";

         QSqlQuery query;
         query.prepare( "INSERT INTO EMLFILES (composed, from_text, from_email, to_text, to_email, subject, receive_path, attachment, md5_hash) "
                         "VALUES (:1, :2, :3, :4, :5, :6, :7, :8, :9)");
         query.bindValue(":1", composed_date);
         query.bindValue(":2", from_text);
         query.bindValue(":3", from_email);
         query.bindValue(":4", to_text);
         query.bindValue(":5", to_email);
         query.bindValue(":6", subject);
         query.bindValue(":7", received_path);
         query.bindValue(":8", attachment);
         query.bindValue(":9", md5_hash);

         if(!query.exec()) {
             qDebug() << query.lastError() << endl;
         }


         for(int k = 0; k < 10; k++){
            Qt::ItemFlags flags = newItem[k]->flags();
            flags |= Qt::ItemIsSelectable;
            flags &= ~Qt::ItemIsEditable; // reset/clear the flag
            newItem[k]->setFlags(flags);
            newItem[k]->setData(Qt::UserRole, i);
            tableWidget->setItem(i, k, newItem[k]);
         }

     }

     db->close();

}

void MainWindow::exportCSV(){
    QString filename = QFileDialog::getSaveFileName(this, "SAVE FILE", QDir::currentPath(), "csv (*.csv)");

    string filepath = filename.toLocal8Bit();
    ofstream file;
    file.open(filepath);
    if(file.fail())
        qDebug() << "SAVE FAIL: file open fail" << endl;

    file << "composed date" << ",";
    file << "from text" << ",";
    file << "from email" << ",";
    file << "to text" << ",";
    file << "to email" << ",";
    file << "subject" << ",";
    file << "receive path" << ",";
//    file << "content" << ",";
    file << "attachment" << ",";
    file << "md5 hash" << endl;

    for(int i = 0; i < tableWidget->rowCount(); i++) {
        file << csv_string_format(metas[i]->composed_date) << ",";
        file << csv_string_format(metas[i]->from_text) << ",";
        file << csv_string_format(metas[i]->from_email) << ",";
        file << csv_string_format(metas[i]->to_text) << ",";
        file << csv_string_format(metas[i]->to_email) << ",";
        file << csv_string_format(metas[i]->subject) << ",";

        string temp = "";
        for(vector<received>::iterator iter = metas[i]->receive_list.begin(); iter != metas[i]->receive_list.end(); iter++){
            string by = iter->by[ip], from = iter->from[ip];
            if(by == "") {
                by = iter->by[word];
            }
            if(from == "") {
                from = iter->from[word];
            }
            temp += "(" + by + " <- " + from + ") ";
        }
        file << csv_string_format(temp) << ",";

        file << contents[i]->attachments.size() << ",";

        QString md5_hash = "";
        for(list<struct file_container>::iterator iter = contents[i]->attachments.begin(); iter != contents[i]->attachments.end(); iter++){
            md5_hash += QString::fromLocal8Bit(iter->id_or_name.c_str()) + ":" + QString::fromLocal8Bit(iter->md5_hash.c_str()) + " ";
        }
        string md5_local_string = md5_hash.toLocal8Bit();
        file << md5_local_string << endl;
    }

    file.close();
}

void MainWindow::click_view(QTableWidgetItem * item){

    if(item->column() == 7) {
        content_viewer = new QPlainTextEdit;
        content_viewer->appendPlainText(QString::fromLocal8Bit(contents[item->row()]->content.c_str()));
        content_viewer->resize(400, 300);
        content_viewer->show();
        return;
    }

    if(item->column() == 8) {
        size_t size = contents[item->data(Qt::UserRole).toInt()]->attachments.size();
        if(size == 0)
            return;
        QDir dir = QFileDialog::getExistingDirectory(this, "select directory", R"(C:\Users\LSJ\Desktop)", QFileDialog::ShowDirsOnly);
        qDebug() << dir.absolutePath() << endl;

        ofstream file;
        string dir_path = dir.absolutePath().toLocal8Bit();
        list<struct file_container>::iterator iter;
        for(iter = contents[item->row()]->attachments.begin(); iter != contents[item->row()]->attachments.end(); iter++) {
            string file_path = dir_path + "\\" + iter->id_or_name;
            file.open(file_path, ios::trunc|ios::binary);

            file << iter->file;

            file.close();
        }
    }

}


