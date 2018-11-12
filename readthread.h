#ifndef READTHREAD_H
#define READTHREAD_H

#include <QThread>
#include "NLcpfw.h"
#include "NLcpfw_st.h"
#include "tcpclient.h"

class ReadThread : public QThread
{
    Q_OBJECT
public:
    void run();

public slots:
    void outputWxInvoice(QByteArray);

signals:
    void wxInvoiceRead(unsigned char);

};

#endif // READTHREAD_H
