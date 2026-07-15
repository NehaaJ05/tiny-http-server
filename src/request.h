#ifndef REQUEST_H
#define REQUEST_H

#include <string>

class HttpRequest
{
public:

    HttpRequest(const std::string& rawRequest);

    std::string getMethod() const;

    std::string getPath() const;

    std::string getVersion() const;

private:

    std::string method;

    std::string path;

    std::string version;

    void parse(const std::string& rawRequest);
};

#endif