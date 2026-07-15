#include "server.h"
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

Server::Server(int port)
{
    this->port = port;
    serverSocket=-1;
}

void Server::start()
{
    std::cout << "Starting server... " << std::endl;
    serverSocket=socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket <0){
        std::cerr << "Failed to create socket. :( " <<std::endl;
        return;
    }
    std::cout << "Socket created successfully. :D " <<std::endl;
}