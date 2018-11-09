#include "mainwindow.h"
#include <QApplication>
#include <QLibrary>
#include <QAction>
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <windows.h>
#include "synchapi.h"

#include "readthread.h"
#include "tcpclient.h"

QAction *action_quit;
HNLCPFW hDev;

static bool checkOne()
{
    //  创建互斥量
    HANDLE m_hMutex  =  CreateMutex(NULL, FALSE,  L"fortest_abc123" );
    //  检查错误代码
    if  (GetLastError()  ==  ERROR_ALREADY_EXISTS)  {
        //  如果已有互斥量存在则释放句柄并复位互斥量
        CloseHandle(m_hMutex);
        m_hMutex  =  NULL;
        //  程序退出
        return  false;
    }
    else
        return true;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if(!checkOne())
    {
        QMessageBox::information(NULL, "Reboot", "The SoftWare is running already");
        return 0;
    }

    QFile file("old.exe");
    if (file.exists())
    {
        file.remove();
    }

    TcpClient *tcpClient = new TcpClient();


    action_quit = new QAction(NULL);
    action_quit->setText(QString("Quit"));
    MainWindow w;
    action_quit->setParent(&w);
    QObject::connect(action_quit, SIGNAL(triggered()), &a, SLOT(quit()));

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
