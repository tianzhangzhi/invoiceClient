#include <QDebug>
#include <QLibrary>
#include "readthread.h"

extern int len;
extern unsigned char btData[];

/*
 *取得合法的UTF8的长度，防止汉字等双字节字符被截断
 */
static int getValidLen(const unsigned char* buff, int len)
{
    int result = 0;
    int increament;
    while (true)
    {
        if(*buff < 128)
            increament = 1;
        else if(((*buff) & 0xf8) == 0xf0)
            increament = 4;
        else if(((*buff) & 0xf0) == 0xe0)
            increament = 3;
        else if(((*buff) & 0xe0) == 0xc0)
            increament = 2;
        else
            return 0;
        if(result + increament > len)
            return result;
        else
        {
            result += increament;
            buff += increament;
        }
    }
}
//→
void ReadThread::run()
{
    QLibrary mylib("user32.DLL");
    if(mylib.load())
    {
        qDebug() << "load ok";
    }
    else
    {
        qDebug() << "load NG";
    }

    HNLCPFW hDev = cpfw_open(L"hid", L"NULL", CPFW_OM_KEEP);
    if(hDev == NULL)
    {
        qDebug() << "open NG";
    }
    while(1)
    {
        len = cpfw_read(hDev, btData, 1, 5000, TRUE);
        if(len > 0)
        {
            //qDebug() << "received" << len;
            len += cpfw_read(hDev, &btData[len], 4000, 300, FALSE);
            len += cpfw_read(hDev, &btData[len], 4000, 100, FALSE);
            len += cpfw_read(hDev, &btData[len], 4000, 60, FALSE);
            len += cpfw_read(hDev, &btData[len], 4000, 20, FALSE);
            len += cpfw_read(hDev, &btData[len], 4000, 10, FALSE);





            // 获取创建前台窗口的线程
            DWORD dwThread = GetWindowThreadProcessId(GetForegroundWindow(), NULL);
            // 将前台窗口线程贴附到当前线程
            AttachThreadInput(dwThread, GetCurrentThreadId(), TRUE);
            // 获取焦点窗口句柄
            HWND hFocus = GetFocus();
            // 解除贴附
            //AttachThreadInput(dwThread, GetCurrentThreadId(), FALSE);

            QString s = QString::fromUtf8((const char *)btData, len);
            //qDebug() << len;
            QStringList list = s.split("→");

            // 发送消息
            unsigned int value;

            if(list.length() == 6)//支付宝发票
            {
                for(int i = 0; i < 4; i++)
                {
                    s = list.at(i);
                    for(int j = 0; j < s.length(); j++)
                    {
                        if(j == 0 || (j == 1 && i == 0))
                            continue;
                        value = s.at(j).unicode();
                        PostMessage(hFocus, WM_CHAR, value , 0);
                    }
                    if(i < 3)
                    {
                        PostMessage(hFocus,WM_SYSKEYDOWN,VK_TAB,0);
                        PostMessage(hFocus,WM_SYSKEYUP,VK_TAB,0);
                        QThread::msleep(100);

                        hFocus = GetFocus();
                    }
                }
            }
            else
            {
                for(int i = 0; i < s.length(); i++)
                {
                    value = s.at(i).unicode();
                    PostMessage(hFocus, WM_CHAR, value , 0);
                }
            }
        }
    }
}
