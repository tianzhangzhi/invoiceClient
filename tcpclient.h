#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>

//#define IPADD "148.70.107.224" //腾讯云服务器IP地址
#define IPADD "192.168.0.251"
#define PORT 10000
#define CUR_VERSION 2

class TcpClient : public QObject
{
    Q_OBJECT
public:
    TcpClient();
    void sendMsg(unsigned char *msg, uint len);
    bool isTcpConnected();


private:
    enum MsgType{VERSION, APP, INVOICE};
    QTcpSocket *client;

    bool hasNewVersion();
    void upDateVersion();

private slots:
    void handleConnection();
    void readMsg();

signals:
    void wxInvoiceGetted(QByteArray);
};

#endif // TCPCLIENT_H
