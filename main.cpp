#include "daemon.h"

int main(int argc, char *argv[])
{
    Daemon *daemon = new Daemon(argc, argv);

    return daemon->exec();
}
