#ifndef DAEMON_H
#define DAEMON_H

#include "server.h"
#include "qtservice.h"

class Daemon : public QtService<QCoreApplication>
{
public:
    Daemon(int argc, char **argv);
    ~Daemon();

    void start();
    void pause();
    void resume();
    void stop();

private:
    Server *server;
};

#endif // DAEMON_H
