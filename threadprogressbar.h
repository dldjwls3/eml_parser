#ifndef THREADPROGRESSBAR_H
#define THREADPROGRESSBAR_H

#include <QProgressBar>
#include <QThread>

class ThreadProgressBar : public QThread
{
public:
    QProgressBar *progressBar;
    ThreadProgressBar(QWidget * parent, int max) {
        progressBar = new QProgressBar(parent);
        progressBar->setVisible(true);
        progressBar->setMinimum(0);
        progressBar->setMaximum(max);

        this->setTerminationEnabled(true);
        this->start();
    }

    void run()
    {
        while(true)
        {
            progressBar->update();
            this->msleep(100);
        }
    }

};

#endif // THREADPROGRESSBAR_H
