#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPlainTextEdit>
#include <QProgressBar>
#include "eml.h"
#include "content_parser.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

signals:
    void numParsedEML(int num);

private slots:
    void open();
    void content_view(QTableWidgetItem * item);

public slots:


private:
    content_parser **contents;
    eml_metadata **metas;

    void createActions();
    QProgressBar *progressBar;
    QTableWidget *tableWidget;
    QPlainTextEdit *content_viewer;
};

#endif // MAINWINDOW_H
