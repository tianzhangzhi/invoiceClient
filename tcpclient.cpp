#include "tcpclient.h"
#include <QHostAddress>
#include <QThread>
#include <QDebug>
#include <QObject>
#include <QFile>
#include <QMessageBox>
#include <QApplication>


static char contentBuf[1024*1024];
uint remain = 0;
uint invoiceRemain = 0;
uint size = 0;
uint read = 0;
static inline uint toInt(uchar *buf)
{
    return (buf[0] << 24) + (buf[1] << 16) + (buf[2] << 8) + (buf[3]);
}

static void updateApp()
{
    QFile file("newV");
    file.open(QIODevice::WriteOnly);
    uint write = file.write(contentBuf, size);
    qDebug() << "writefile" << write << size;
    file.flush();
    file.close();
    QFile oldfile("postmessage.exe");
    bool ok = oldfile.rename("old.exe");
    oldfile.close();
    QFile newfile("newV");
    ok = newfile.rename("postmessage.exe");
    newfile.close();
    QMessageBox::information(NULL, QObject::tr("update"),
                                           QString(QObject::tr("Software has updated, Reboot Please!")));
    QApplication::exit();
}

TcpClient::TcpClient()
{
    client = new QTcpSocket(NULL);
    QObject::connect(client, SIGNAL(connected()), this, SLOT(handleConnection()));
    QObject::connect(client, SIGNAL(readyRead()), this, SLOT(readMsg()));
    client->connectToHost(QHostAddress(IPADD), PORT);
    client->waitForConnected();
}

void TcpClient::handleConnection()
{

}

void TcpClient::readMsg()
{
    qDebug() << "readmsg";
    char aChar;
    QByteArray resp;
    if(remain > 0)
        goto readApp;
    if(invoiceRemain > 0)
        goto readInvoice;
    client->read(&aChar, 1);
    qDebug() << "read respCode" << aChar;
    switch (aChar) {
    case VERSION:
        client->read(&aChar, 1);
        if(aChar > CUR_VERSION)
            client->write(QByteArray(1, 1));
        else
            client->write(QByteArray(1, 0));
        break;
    case APP:
    {
        char sizeBuf[4];
        client->read(sizeBuf, 4);
        size = toInt((uchar *)sizeBuf);
        read = 0;
        remain = size;
        qDebug() << size;

readApp:
        read += client->read(&contentBuf[read], remain);
        remain = size - read;
        qDebug() << "net read" << read << remain;
        if(remain == 0)
            updateApp();
    }
        break;
    case INVOICE:
        char sizeBuf[4];
        client->read(sizeBuf, 4);
        size = toInt((uchar *)sizeBuf);
        qDebug() << "invoice Len" << size;
        read = 0;
        invoiceRemain = size;
readInvoice:
        read += client->read(&contentBuf[read], invoiceRemain);
        invoiceRemain = size - read;
        qDebug() << "invoice read" << read << remain;
        if(invoiceRemain == 0) {
            //qDebug() << "read invoice" << resp.length() << QString::fromUtf8(contentBuf, size);
            emit wxInvoiceGetted(QByteArray(contentBuf, size));
        }
        break;
    default:
        break;
    }
}

bool TcpClient::hasNewVersion()
{
    char version[10] = {'1', '2'};
    client->write(version, 2);
    int read = client->read(version, 3);
    qDebug() << "tcp read" << read;
    return version[0] > VERSION;
}

void TcpClient::upDateVersion()
{
    qDebug() << "updating";
}

void TcpClient::sendMsg(unsigned char *msg, uint len)
{
    uint write = client->write((const char*)msg, len);
    qDebug() << "write " << write;
}

bool TcpClient::isTcpConnected()
{
    qDebug() << client->state();
    return client->state() == QAbstractSocket::ConnectedState;
}
