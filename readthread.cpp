#include <QDebug>
#include <QLibrary>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include "readthread.h"


unsigned char btData[4000];

extern HNLCPFW hDev;

static uint CP1251Table[64] = {
    0x0402, 0x0403, 0x201A, 0x0453, 0x201E, 0x2026, 0x2020, 0x2021, 0x20AC, 0x2030, 0x0409, 0x2039, 0x040A, 0x040C, 0x040B, 0x040F,
    0x0452, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014, 0x0000, 0x2122, 0x0459, 0x203A, 0x045A, 0x045C, 0x045B, 0x045F,
    0x00A0, 0x040E, 0x045E, 0x0408, 0x00A4, 0x0490, 0x00A6, 0x00A7, 0x0401, 0x00A9, 0x0404, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x0407,
    0x00B0, 0x00B1, 0x0406, 0x0456, 0x0491, 0x00B5, 0x00B6, 0x00B7, 0x0451, 0x2116, 0x0454, 0x00BB, 0x0458, 0x0405, 0x0455, 0x0457
};

static void outputString(QString s)
{
    // 获取创建前台窗口的线程
    DWORD dwThread = GetWindowThreadProcessId(GetForegroundWindow(), NULL);
    // 将前台窗口线程贴附到当前线程
    AttachThreadInput(dwThread, GetCurrentThreadId(), TRUE);
    // 获取焦点窗口句柄
    HWND hFocus = GetFocus();
    for(int j = 0; j < s.length(); j++)
    {
        ushort value = s.at(j).unicode();
        //QThread::msleep(10);
        PostMessage(hFocus, WM_CHAR, value , 1);
        //QThread::msleep(10);
    }
}

static void outputTab()
{
    // 获取创建前台窗口的线程
    DWORD dwThread = GetWindowThreadProcessId(GetForegroundWindow(), NULL);
    // 将前台窗口线程贴附到当前线程
    AttachThreadInput(dwThread, GetCurrentThreadId(), TRUE);
    // 获取焦点窗口句柄
    HWND hFocus = GetFocus();
    PostMessage(hFocus,WM_SYSKEYDOWN,VK_TAB,1);
    QThread::msleep(100);
    PostMessage(hFocus,WM_SYSKEYUP,VK_TAB,1);
    QThread::msleep(100);
}

static bool isWxInvoice(unsigned char *buff)
{
    bool result = false;
    const char* ident = "https://w.url.cn/s/";
    unsigned char index = strlen(ident);
    unsigned char tmp = buff[index];
    buff[index] = 0;
    if(strcmp(ident, (const char*)buff) == 0)
        result = true;
    buff[index] = tmp;
    return result;
}

static QString fromCP1251(unsigned char *buff, int len)
{
    uint uniStr[len + 1];
    uniStr[len] = 0;
    for(int i = 0; i < len; i++)
    {
        if(buff[i] < 128)
            uniStr[i] = buff[i];
        else if(buff[i] > 191)
            uniStr[i] = buff[i] + 0x350;
        else
            uniStr[i] = CP1251Table[buff[i] - 128];
    }
    return QString::fromUcs4(uniStr);
}

void ReadThread::outputWxInvoice(QByteArray content)
{
    //qDebug() << "content" << content;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(content);
    if( jsonDocument.isNull() ){
        qDebug()<< "===> QJsonDocument："<< content;
    }

    QJsonObject jsonObject = jsonDocument.object();

    outputString(jsonObject.take("title").toString());
    outputTab();

    outputString(jsonObject.take("tax_no").toString());
    outputTab();

    outputString(jsonObject.take("addr").toString() + " "
                 + jsonObject.take("phone").toString());
    outputTab();

    outputString(jsonObject.take("bank_type").toString() + " "
                 + jsonObject.take("bank_no").toString());
}

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

    int len;
    while(1)
    {
        len = cpfw_read(hDev, btData, 100, 1000, FALSE);
        qDebug() << "received" << len;
        if(len > 0)
        {
            //qDebug() << "received" << len;
            len += cpfw_read(hDev, &btData[len], 100, 10, FALSE);
            len += cpfw_read(hDev, &btData[len], 100, 10, FALSE);
            len += cpfw_read(hDev, &btData[len], 100, 10, FALSE);
            len += cpfw_read(hDev, &btData[len], 100, 10, FALSE);
            len += cpfw_read(hDev, &btData[len], 100, 10, FALSE);
            len += cpfw_read(hDev, &btData[len], 100, 10, FALSE);
            len += cpfw_read(hDev, &btData[len], 100, 10, FALSE);
            len += cpfw_read(hDev, &btData[len], 100, 10, FALSE);
            len += cpfw_read(hDev, &btData[len], 100, 10, FALSE);
            len += cpfw_read(hDev, &btData[len], 100, 10, FALSE);
            len += cpfw_read(hDev, &btData[len], 100, 10, FALSE);
            len += cpfw_read(hDev, &btData[len], 100, 10, FALSE);
            len += cpfw_read(hDev, &btData[len], 100, 10, FALSE);
            len += cpfw_read(hDev, &btData[len], 100, 10, FALSE);
            len += cpfw_read(hDev, &btData[len], 100, 10, FALSE);

            // 获取创建前台窗口的线程
            DWORD dwThread = GetWindowThreadProcessId(GetForegroundWindow(), NULL);
            // 将前台窗口线程贴附到当前线程
            AttachThreadInput(dwThread, GetCurrentThreadId(), TRUE);
            // 获取焦点窗口句柄
            HWND hFocus = GetFocus();
            // 解除贴附
            //AttachThreadInput(dwThread, GetCurrentThreadId(), FALSE);

            if(isWxInvoice(btData))
            {
                emit wxInvoiceRead((unsigned char)len);
                continue;
            }

            QString s;
            if(btData[0] == 0x5c && btData[1] == 0x30 && btData[2] == 0x30 && btData[3] == 0x30
                     && btData[4] == 0x30 && btData[5] == 0x32 && btData[6] == 0x32)
                s = fromCP1251(&btData[7], len - 7);
            else if(btData[0] == 0x5c && btData[1] == 0x30 && btData[2] == 0x30 && btData[3] == 0x30
                    && btData[4] == 0x30 && btData[5] == 0x32 && btData[6] == 0x36)
                s = QString::fromUtf8((const char *)&btData[7], len - 7);
            else
                s = QString::fromUtf8((const char *)btData, len);
            //QString s = QString::fromUtf8((const char *)btData, len);
            //QString s = fromCP1251(btData, len);
            //qDebug() << len;
            QStringList list = s.split("→");

            // 发送消息
            unsigned int value;

            if(list.length() == 6)//支付宝发票
            {
                for(int i = 0; i < 4; i++)
                {
                    s = list.at(i);
                    if(i == 0)
                        s = s.right(s.length() - 2);
                    else
                        s = s.right(s.length() - 1);
                    outputString(s);
                    if(i < 3)
                    {
                        outputTab();
                    }
                }
            }
            else
            {
                outputString(s);
            }
        }
    }
}
