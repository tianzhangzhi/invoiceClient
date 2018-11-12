#ifndef TCPTHREAD_H
#define TCPTHREAD_H

#include <QThread>

class TcpThread : public QThread
{
    Q_OBJECT
public:
    TcpThread();
    void run();

public slots:
    void sendWxInvoice(unsigned char);
};

#endif // TCPTHREAD_H
