#include "daemon.h"

Daemon::Daemon(int argc, char **argv) : QtService<QCoreApplication>(argc, argv, "Setting_App_SC")
{
    try{
        setServiceDescription("This is the Service for Setting_App");
        setServiceFlags(QtServiceBase::CanBeSuspended);
    }catch(...){
        logMessage("An unknown error in the constructor");
    }
}

Daemon::~Daemon()
{
    try{
        delete server;
    }catch(...){
        logMessage("An unknown error in the deconstructor");
    }
}

void Daemon::start()
{
    try {
        QCoreApplication *app = application();

        this->server = new Server(app);

        if (!server->isListening()) {
            logMessage(QString("Failed to bind to port 9050"), QtServiceBase::Error);
            app->quit();
        }
    } catch(...)
    {
        logMessage("An unknown error in the start");
    }
}

void Daemon::pause() {
    try{
        server->pause();
        logMessage("Server pause");
    }catch(...)
    {
        logMessage("An unknown error in the pause");
    }
}

void Daemon::resume() {
    try{
        server->resume();
        logMessage("Server resumed");
    }catch(...)
    {
        logMessage("An unknown error in the resume");
    }
}

void Daemon::stop() {
    try{
        server->close();
        logMessage("Server stopped");
    }catch(...)
    {
        logMessage("An unknown error in the stop");
    }
}
