#include "mainwindow.h"
#include <QApplication>
#include <QLibrary>
#include <QDebug>
#include "synchapi.h"

#include "NLcpfw.h"
#include "NLcpfw_st.h"


void NLCPFW_API RecvDataEvent(void *pBuf, UINT dwDataLen, void *pParam)
{
    QString s = QString::fromUtf8((const char *)pBuf);
    HWND hWnd = GetForegroundWindow();
    printf("");
    qDebug() << s;
    unsigned int value = s.at(10).unicode();

    // 获取创建前台窗口的线程
    DWORD dwThread = GetWindowThreadProcessId(GetForegroundWindow(), NULL);
    // 将前台窗口线程贴附到当前线程（也就是程序A中的调用线程）
    AttachThreadInput(dwThread, GetCurrentThreadId(), TRUE);
    // 获取焦点窗口句柄
    HWND hFocus = GetFocus();
    // 解除贴附
    //AttachThreadInput(dwThread, GetCurrentThreadId(), FALSE);
    // 发送消息
    PostMessage(hFocus, WM_CHAR, value , 0);
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QLibrary mylib("user32.DLL");
    if(mylib.load())
    {
        qDebug() << "load ok";
    }
    else
    {
        qDebug() << "load NG";
        return -2;
    }

    HNLCPFW hDev = cpfw_open(L"hid", L"NULL", CPFW_OM_KEEP);
    if(hDev == NULL)
    {
        printf("open error!");
        return -1;
    }
    cpfw_register_notify(hDev, NULL, RecvDataEvent, NULL);

    MainWindow w;
    w.show();

    return a.exec();
}
