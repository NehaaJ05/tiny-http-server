#ifndef MIME_H
#define MIME_H

#include <string>

class Mime
{
public:
    static std::string getContentType(const std::string& filePath);
};

#endif