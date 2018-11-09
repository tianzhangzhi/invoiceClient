#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>

//#define IPADD "148.70.107.224" //腾讯云服务器IP地址
#define IPADD "192.168.0.251"
#define PORT 10000
#define CUR_VERSION 1

class TcpClient : public QObject
{
    Q_OBJECT
public:
    TcpClient();

private:
    enum MsgType{VERSION, APP, INVOICE};
    QTcpSocket *client;

    bool hasNewVersion();
    void upDateVersion();

private slots:
    void handleConnection();
    void readMsg();
};

#endif // TCPCLIENT_H
