#include "server.h"
#include "socketthread.h"
#include <QDebug>
#include <QFile>
#include <QDataStream>
#include <QSettings>

Server::Server(QObject* parent) : QTcpServer(parent)
{
    this->disabled = false;

    loadSettings();

    startServer();
}

void Server::startServer(){
    if (!this->listen(QHostAddress::AnyIPv4, 9050)) {
        qDebug() << "Could not start server at port: " << this->serverPort();
    } else {
        qDebug() << "Listening...";
    }
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    if (disabled)
        return;

    // When a new client connects, the server constructs a QTcpSocket and all
    // communication with the client is done over this QTcpSocket.

    SocketThread *socket = new SocketThread(socketDescriptor, this);

    connect(socket, SIGNAL(finished()), socket, SLOT(deleteLater()));
    connect(socket, SIGNAL(emitDataLogin(QStringList)), this, SLOT(updateData(QStringList)));
    connect(socket, SIGNAL(sendToClient(QTcpSocket*)), this, SLOT(sendToClient(QTcpSocket*)));

    socket->start();
}

void Server::updateData(QStringList data){
    this->hostname = data.at(0);
    this->port = data.at(1).toInt();
    this->databasename = data.at(2);
    this->username = data.at(3);
    this->password = data.at(4);

    saveSettings();
}

void Server::sendToClient(QTcpSocket *socket)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_1);

    out << (quint16)0;

    out << hostname << port << databasename << username << password;

    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    socket->write(block);
    socket->flush();

    QtServiceBase::instance()->logMessage("Wrote to client");
}

void Server::saveSettings(){
    QSettings settings("SGC", "Setting");

    settings.beginGroup("Database");
    settings.setValue("hostName", hostname);
    settings.setValue("dbName", databasename);
    settings.setValue("port", port);
    settings.setValue("userName", username);
    settings.setValue("pswdName", password);
    settings.endGroup();
}

void Server::loadSettings(){
    QSettings settings("SGC", "Setting");

    settings.beginGroup("Database");
    QString hostName = settings.value("hostName").toString();
    QString dbName = settings.value("dbName").toString();
    int port = settings.value("port").toInt();
    QString userName = settings.value("userName").toString();
    QString pswdName = settings.value("pswdName").toString();
    settings.endGroup();

    this->hostname = hostName;
    this->databasename = dbName;
    this->port = port;
    this->username = userName;
    this->password = pswdName;
}

