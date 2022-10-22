#include "socketthread.h"
#include <QDataStream>
#include <QDebug>
#include <QFile>

SocketThread::SocketThread(qintptr socket_descriptor, QObject *parent)
    :  QThread(parent), socket_descriptor(socket_descriptor)
{

}

void SocketThread::run()
{
    socket = new QTcpSocket;
    blockSize = 0;
    if(!socket->setSocketDescriptor(this->socket_descriptor)){
        emit error(socket->error());
        return;
    }

    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()), Qt::DirectConnection);

    exec();
}

void SocketThread::onReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_1);

    if(blockSize == 0){
        if(socket->bytesAvailable() < (int)sizeof(quint16))
            return;
        in >> blockSize;
    }
    if(socket->bytesAvailable() < blockSize)
        return;

    QString type;

    in >> type;

    if(type == "SGC")
        emit sendToClient(this->socket);
    else if(type == "Setting_App"){

        QString hostname, databasename, username, password;
        int port;

        in >> hostname >> port >> databasename >> username >> password;

        QStringList data;
        data.append(hostname);
        data.append(QString::number(port));
        data.append(databasename);
        data.append(username);
        data.append(password);

        emit emitDataLogin(data);
    }
    QtServiceBase::instance()->logMessage("Recive from client");

    if (socket->state() == QTcpSocket::UnconnectedState) {
        delete socket;
        QtServiceBase::instance()->logMessage("Connection closed");
    }
}

void SocketThread::onDisconnected()
{
    QtServiceBase::instance()->logMessage(QString::number(socket_descriptor) + " Disconnected");

    socket->deleteLater();

    QtServiceBase::instance()->logMessage("Connection closed");
    exit(0);
}
