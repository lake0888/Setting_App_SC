#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H

#include <QThread>
#include <QObject>
#include <QTcpSocket>
#include <QStringList>

#include "qtservice.h"

class SocketThread : public QThread
{
    Q_OBJECT
public:
    SocketThread(qintptr socket_descriptor, QObject *parent = nullptr);
    QTcpSocket *get_Socket() { return this->socket; }

protected:
    void run() Q_DECL_OVERRIDE;

signals:
    void error(QTcpSocket::SocketError socketerror);
    void emitDataLogin(QStringList);
    void sendToClient(QTcpSocket *);

private slots:
    void onReadyRead();
    void onDisconnected();

private:
    QTcpSocket *socket;
    qintptr socket_descriptor;
    quint16 blockSize;
};

#endif // SOCKETTHREAD_H
