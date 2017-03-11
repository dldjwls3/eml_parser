#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <errno.h>
#include "eml.h"
#include "regex_set.h"
#include "content_parser.h"
#include "threadprogressbar.h"

#include "mainwindow.h"
#include <QtWidgets>
#include <QDebug>
#include <list>



void MainWindow::createActions(){
    QMenu *fileMenu = menuBar()->addMenu("&File");

    QAction *openAct = new QAction("&Open", this);
    openAct->setShortcut(QKeySequence::Open);
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    fileMenu->addAction(openAct);

}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    tableWidget = new QTableWidget(this);
    tableWidget->setSortingEnabled(true);
    setCentralWidget(tableWidget);
    //setAcceptDrops(true);
    createActions();
    connect(tableWidget, &QTableWidget::itemDoubleClicked, this, &MainWindow::content_view);

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


     tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("compoesd date"));
     tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("from text"));
     tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("from email"));
     tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("to text"));
     tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem("to email"));
     tableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem("subject"));     
     tableWidget->setHorizontalHeaderItem(6, new QTableWidgetItem("recevie path"));
     tableWidget->setHorizontalHeaderItem(7, new QTableWidgetItem("content"));
     tableWidget->setHorizontalHeaderItem(8, new QTableWidgetItem("attachment"));
     tableWidget->setHorizontalHeaderItem(9, new QTableWidgetItem("md5 hash"));


     progressBar = new QProgressBar;
     progressBar->setVisible(true);
     progressBar->setMinimum(0);
     progressBar->setMaximum(row);

     for(int i = 0; i < row; i++) {
         QString fileName = filenames.at(i);
         std::string path = fileName.toLocal8Bit().constData();

         //string path = R"(C:\Users\LSJ\Documents\Qt\eml_parser-master\eml sample\20140826_36_webinars@tmcnet.com.eml)";

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

         //eml_file.print_dic();
         //eml_file.print_receive_list();
         //eml_file.meta.print_meta();

         file.close();

         QTableWidgetItem *newItem[10];

         newItem[0] = new QTableWidgetItem(QString::fromLocal8Bit(metas[i]->composed_date.c_str()));
         newItem[1] = new QTableWidgetItem(QString::fromLocal8Bit(metas[i]->from_text.c_str()));
         newItem[2] = new QTableWidgetItem(QString::fromLocal8Bit(metas[i]->from_email.c_str()));
         newItem[3] = new QTableWidgetItem(QString::fromLocal8Bit(metas[i]->to_text.c_str()));
         newItem[4] = new QTableWidgetItem(QString::fromLocal8Bit(metas[i]->to_email.c_str()));
         newItem[5] = new QTableWidgetItem(QString::fromLocal8Bit(metas[i]->subject.c_str()));

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
         newItem[6] = new QTableWidgetItem(QString::fromLocal8Bit(temp.c_str()));

         newItem[7] = new QTableWidgetItem(QString::fromLocal8Bit((eml_file.content.content.substr(0, 30) + "...").c_str()));
         newItem[8] = new QTableWidgetItem(QString::number(eml_file.content.attachments.size()));

         QString md5_hash = "";
         for(list<struct file_container>::iterator iter = eml_file.content.attachments.begin(); iter != eml_file.content.attachments.end(); iter++){
             md5_hash += QString::fromLocal8Bit(iter->id_or_name.c_str()) + ": " + QString::fromLocal8Bit(iter->md5_hash.c_str()) + "\n";
         }
         newItem[9] = new QTableWidgetItem(md5_hash);


         for(int k = 0; k < 10; k++){
            Qt::ItemFlags flags = newItem[k]->flags();
            flags |= Qt::ItemIsSelectable;
            flags &= ~Qt::ItemIsEditable; // reset/clear the flag
            newItem[k]->setFlags(flags);
            tableWidget->setItem(i, k, newItem[k]);
         }

         progressBar->setValue(i + 1);
         progressBar->update();
     }



}

void MainWindow::content_view(QTableWidgetItem * item){

    if(item->column() == 7) {
        content_viewer = new QPlainTextEdit;
        content_viewer->appendPlainText(QString::fromLocal8Bit(contents[item->row()]->content.c_str()));
        content_viewer->resize(400, 300);
        content_viewer->show();
        return;
    }

    if(item->column() == 8) {
        size_t size = contents[item->row()]->attachments.size();
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
