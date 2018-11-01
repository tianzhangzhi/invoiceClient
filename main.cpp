#include "mainwindow.h"
#include <QApplication>
#include <QLibrary>
#include <QAction>
#include <QDebug>
#include <QMessageBox>
#include "synchapi.h"

#include "readthread.h"

QAction *action_quit;
HNLCPFW hDev;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    action_quit = new QAction(NULL);
    action_quit->setText(QString("退出"));
    QObject::connect(action_quit, SIGNAL(triggered()), &a, SLOT(quit()));
    MainWindow w;
    w.show();

retry:
    hDev = cpfw_open(L"hid", L"NULL", CPFW_OM_KEEP);
    if(hDev == NULL)
    {

        int button;
        button = QMessageBox::question(&w, QObject::tr("打开失败"),
                                       QString(QObject::tr("重试?")),
                                       QMessageBox::Yes | QMessageBox::No);
        if (button == QMessageBox::No) {
              return -1;
        }
        else if (button == QMessageBox::Yes) {
              goto retry;
        }

    }

    ReadThread *readThred = new ReadThread();
    readThred->start();

    return a.exec();
}
