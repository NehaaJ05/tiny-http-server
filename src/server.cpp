#include "server.h"
#include "response.h"
#include "request.h"
#include "router.h"
#include "mime.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>
#include <thread>
#include <chrono>

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
    int option = 1;

    if (setsockopt(
            serverSocket,
            SOL_SOCKET,
            SO_REUSEADDR,
            &option,
            sizeof(option)) < 0)
    {
        std::cerr << "Failed to configure socket options."
                  << std::endl;

        close(serverSocket);
        return;
    }

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
        std::thread clientThread(
            &Server::handleClient,
            this,
            clientSocket,
            clientAddress
        );

        clientThread.detach();
    }
}

void Server::handleClient(int clientSocket,
                          const sockaddr_in& clientAddress)
{
    std::cout << "Client connected from "
              << inet_ntoa(clientAddress.sin_addr)
              << ":"
              << ntohs(clientAddress.sin_port)
              << std::endl;

    constexpr size_t BUFFER_SIZE = 4096;

    //check if parallel with timer
    std::this_thread::sleep_for(
        std::chrono::seconds(5)
    );
    char buffer[BUFFER_SIZE];

    ssize_t bytesReceived =
        recv(clientSocket,
             buffer,
             BUFFER_SIZE - 1,
             0);

    if (bytesReceived <= 0)
    {
        close(clientSocket);
        return;
    }

    buffer[bytesReceived] = '\0';

    HttpRequest request(buffer);

    std::cout << "\n======= Incoming Request =======\n";

    std::cout << "Method : "
              << request.getMethod()
              << std::endl;

    std::cout << "Path   : "
              << request.getPath()
              << std::endl;

    std::cout << "Version: "
              << request.getVersion()
              << std::endl;
     
     
    Router router;
    std::string filePath =router.getFilePath(request);

    std::string page =readFile(filePath);

    int statusCode = 200;
    std::string statusText = "OK";

    if (filePath == "public/404.html")
    {
        statusCode = 404;
        statusText = "Not Found";
    }

    std::string contentType =
    Mime::getContentType(filePath);

    HttpResponse response(
        statusCode,
        statusText,
        contentType,
        page
    );

    std::string httpResponse = response.build();

    send(clientSocket,
         httpResponse.c_str(),
         httpResponse.length(),
         0);

    close(clientSocket);

    std::cout << "Connection closed."
              << std::endl;
}

std::string Server::readFile(const std::string& filePath)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}
