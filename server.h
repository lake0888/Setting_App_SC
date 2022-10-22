#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QStringList>

class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server(QObject* parent = nullptr);

    void startServer();

    void pause() { disabled = true; }
    void resume() { disabled = false; }

protected:
    void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;

public slots:
    void updateData(QStringList);
    void sendToClient(QTcpSocket *);

private:
    bool disabled;

    QString hostname, databasename, username, password;
    int port;

    void saveSettings();
    void loadSettings();
};

#endif // SERVER_H
