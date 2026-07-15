#include "response.h"

HttpResponse::HttpResponse(int statusCode,
                           const std::string& statusText,
                           const std::string& contentType,
                           const std::string& body)
{
    this->statusCode = statusCode;
    this->statusText = statusText;
    this->contentType = contentType;
    this->body = body;
}

std::string HttpResponse::build() const
{
    return
        "HTTP/1.1 " +
        std::to_string(statusCode) +
        " " +
        statusText +
        "\r\n"

        "Content-Type: " +
        contentType +
        "\r\n"

        "Content-Length: " +
        std::to_string(body.length()) +
        "\r\n"

        "\r\n"

        + body;
}