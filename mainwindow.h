#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPlainTextEdit>
#include <QProgressBar>
#include "eml.h"
#include "content_parser.h"
#include "database.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

signals:
    void numParsedEML(int num);

private slots:
    void open();
    void exportCSV();
    void click_view(QTableWidgetItem * item);


private:
    content_parser **contents;
    eml_metadata **metas;
    database *db;

    void createActions();
    QTableWidget *tableWidget;
    QPlainTextEdit *content_viewer;
};

#endif // MAINWINDOW_H
