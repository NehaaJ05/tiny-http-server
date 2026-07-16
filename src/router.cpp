#include "router.h"

std::string Router::getFilePath(const HttpRequest& request) const
{
    if (request.getPath() == "/")
    {
        return "public/index.html";
    }

    if (request.getPath() == "/about")
    {
        return "public/about.html";
    }

    return "public/404.html";
}