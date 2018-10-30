#include "mainwindow.h"
#include <QApplication>
#include <QLibrary>
#include <QDebug>
#include "synchapi.h"

#include "readthread.h"

unsigned char btData[4000];
int len;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();


    ReadThread *readThred = new ReadThread();
    readThred->start();

    return a.exec();
}
