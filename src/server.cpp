#include "server.h"
#include "response.h"
#include "request.h"
#include "router.h"
#include "mime.h"
#include "logger.h"
#include "stats.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>
#include <thread>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

Server::Server(int port)
{
    this->port = port;
    serverSocket = -1;
}

void Server::start()
{
    Logger::log("Starting server...");

    // Create TCP socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket < 0)
    {
        Logger::log("ERROR: Failed to create socket.");
        return;
    }

    Logger::log("Socket created successfully.");

    // Allow immediate port reuse
    int option = 1;

    if (setsockopt(serverSocket,
                   SOL_SOCKET,
                   SO_REUSEADDR,
                   &option,
                   sizeof(option)) < 0)
    {
        Logger::log("ERROR: Failed to configure socket options.");
        close(serverSocket);
        return;
    }

    // Configuring server address
    sockaddr_in serverAddress{};

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if (bind(serverSocket,
             reinterpret_cast<sockaddr*>(&serverAddress),
             sizeof(serverAddress)) < 0)
    {
        Logger::log("ERROR: Failed to bind socket.");
        close(serverSocket);
        return;
    }

    Logger::log(
        "Socket bound to port " +
        std::to_string(port)
    );

    // Listen
    if (listen(serverSocket, 5) < 0)
    {
        Logger::log("ERROR: Failed to listen.");
        close(serverSocket);
        return;
    }

    Logger::log(
        "Server listening on port " +
        std::to_string(port)
    );

    acceptConnections();
}

void Server::acceptConnections()
{
    while (true)
    {
        sockaddr_in clientAddress{};
        socklen_t clientLength = sizeof(clientAddress);

        int clientSocket =
            accept(serverSocket,
                   reinterpret_cast<sockaddr*>(&clientAddress),
                   &clientLength);

        if (clientSocket < 0)
        {
            Logger::log("ERROR: Failed to accept connection.");
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

void Server::handleClient(int clientSocket, const sockaddr_in& clientAddress)
{
    Stats::requestStarted();

    Logger::log(
        "Client connected from " +
        std::string(inet_ntoa(clientAddress.sin_addr)) +
        ":" +
        std::to_string(ntohs(clientAddress.sin_port))
    );

    constexpr size_t BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE];

    ssize_t bytesReceived =
        recv(clientSocket,
             buffer,
             BUFFER_SIZE - 1,
             0);

    if (bytesReceived <= 0)
    {
        Logger::log("ERROR: Failed to receive request.");
        Stats::requestFinished();
        close(clientSocket);
        return;
    }

    buffer[bytesReceived] = '\0';

    HttpRequest request(buffer);

    Logger::log(
        request.getMethod() +
        " " +
        request.getPath() +
        " " +
        request.getVersion()
    );

    Router router;

    std::string filePath = router.getFilePath(request);

    std::string page;

    if (filePath == "__stats__")
    {
        page = Stats::getJson();
    }
    else
    {
        page = readFile(filePath);
    }

    int statusCode = 200;
    std::string statusText = "OK";

    if (filePath == "public/404.html")
    {
        statusCode = 404;
        statusText = "Not Found";
    }

    std::string contentType;

    if (filePath == "__stats__")
    {
        contentType = "application/json";
    }
    else
    {
        contentType =
            Mime::getContentType(filePath);
    }
    
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

    Logger::log(
        "Response sent (" + std::to_string(statusCode) + ")"
    );

    Stats::requestFinished();

    close(clientSocket);

    Logger::log("Connection closed.");
}

std::string Server::readFile(const std::string& filePath)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        Logger::log("ERROR: Could not open file: " + filePath );
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

void Server::stop()
{
    Logger::log("Server shutting down.");

    if (serverSocket >= 0)
    {
        close(serverSocket);
    }
}