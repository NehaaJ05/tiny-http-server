#include "server.h"
#include "response.h"
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

    sockaddr_in serverAddress{}; //configuring server addr

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port =htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    //binding socket
    if (bind(serverSocket,reinterpret_cast<sockaddr*>(&serverAddress),sizeof(serverAddress))<0)
    {
        std::cerr << "Failed to bind socket. T^T " << std::endl;
        close(serverSocket);
        return;
    }

    std::cout << "Socket bound to port "
              << port
              << " successfully. " << std::endl;

    if (listen(serverSocket,5)<0){
        std::cerr << "Failed to listen. ¬_¬ " << std::endl;
        close(serverSocket);
        return;
    }
    std::cout << "Server is now listening on port "
              << port
              << std::endl;
    
    acceptConnections();
}

void Server::acceptConnections(){
    while(true){
        sockaddr_in clientAddress{};
        socklen_t clientLength =sizeof(clientAddress);
        int clientSocket = accept(serverSocket,reinterpret_cast<sockaddr*>(&clientAddress),&clientLength);
        if (clientSocket <0){
            std::cerr << "Failed to accept connection. （︶^︶）"
                      << std::endl;
            continue;
        }
        
        std::cout << "Client connected from "
                  << inet_ntoa(clientAddress.sin_addr)
                  << ":"
                  << ntohs(clientAddress.sin_port)
                  << std::endl;
        
        HttpResponse response(
            200,
            "OK",
            "text/plain",
            "Hello from Tiny HTTP Server!"
        );
        std::string httpResponse = response.build();
        send(clientSocket,httpResponse.c_str(),httpResponse.length(),0);
        close(clientSocket);

        std::cout << "Connection closed. Goodbye"
                  << std::endl;
    }
}