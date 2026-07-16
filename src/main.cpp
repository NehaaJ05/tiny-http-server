#include "server.h"
#include "stats.h"

#include <csignal>

Server* serverInstance = nullptr;

void signalHandler(int signal)
{
    if (signal == SIGINT)
    {
        if (serverInstance != nullptr)
        {
            serverInstance->stop();
        }

        exit(0);
    }
}

int main()
{
    Stats::initialize();

    Server server(8080);

    serverInstance = &server;

    signal(SIGINT, signalHandler);

    server.start();

    return 0;
}