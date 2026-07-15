#ifndef RESPONSE_H
#define RESPONSE_H

#include <string>

class HttpResponse
{
public:
    HttpResponse(int statusCode,
                 const std::string& statusText,
                 const std::string& contentType,
                 const std::string& body);

    std::string build() const;

private:
    int statusCode;
    std::string statusText;
    std::string contentType;
    std::string body;
};

#endif