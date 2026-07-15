#include "request.h"

#include <sstream>

HttpRequest::HttpRequest(const std::string& rawRequest)
{
    parse(rawRequest);
}

void HttpRequest::parse(const std::string& rawRequest)
{
    std::istringstream stream(rawRequest);

    stream >> method;
    stream >> path;
    stream >> version;
}

std::string HttpRequest::getMethod() const
{
    return method;
}

std::string HttpRequest::getPath() const
{
    return path;
}

std::string HttpRequest::getVersion() const
{
    return version;
}