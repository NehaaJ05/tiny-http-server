#include "server.h"
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

Server::Server(int port)
{
    this->port = port;
    serverSocket=-1;
}

void Server::start()
{
    std::cout << "Starting server... " << std::endl;

    serverSocket=socket(AF_INET, SOCK_STREAM, 0); //creating a TCP socket

    if (serverSocket <0){
        std::cerr << "Failed to create socket. :( " <<std::endl;
        return;
    }

    std::cout << "Socket created successfully. :D " <<std::endl;

    sockaddr_in serverAddress{};

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port =htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket,reinterpret_cast<sockaddr*>(&serverAddress),sizeof(serverAddress))<0)
    {
        std::cerr << "Failed to bind socket. T^T " << std::endl;
        close(serverSocket);
        return;
    }

    std::cout << "Socket bound to port "
              << port
              << " successfully. " << std::endl;
}