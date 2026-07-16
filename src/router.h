#ifndef ROUTER_H
#define ROUTER_H
#include <string>
#include "request.h"

class Router
{
public:

    std::string getFilePath(const HttpRequest& request) const;

};

#endif