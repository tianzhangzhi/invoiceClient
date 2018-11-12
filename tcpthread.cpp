#include "tcpthread.h"
#include "tcpclient.h"
extern unsigned char btData[4000];
TcpClient *tcpClient = NULL;

TcpThread::TcpThread()
{
    tcpClient = new TcpClient();
}


void TcpThread::run()
{

}

void TcpThread::sendWxInvoice(unsigned char len)
{
    tcpClient->sendMsg(&len, 1);
    tcpClient->sendMsg(btData, len);
}
