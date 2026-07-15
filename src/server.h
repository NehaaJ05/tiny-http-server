#ifndef SERVER_H
#define SERVER_H
#include <string>
#include <netinet/in.h>
#include "request.h"
class Server
{
public:
    Server(int port);
    void start();

private:
    int port;
    int serverSocket;
    void acceptConnections();
    void handleClient(int clientSocket, const sockaddr_in& clientAddress);
    std::string readFile(const std::string& filePath);
    std::string getFilePath(const HttpRequest& request);
};

#endif