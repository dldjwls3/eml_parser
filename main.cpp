


#include <QApplication>
#include "mainwindow.h"


using namespace std;





int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow mainWin;
    mainWin.show();



    return app.exec();
}

