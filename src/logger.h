#ifndef LOGGER_H
#define LOGGER_H

#include <mutex>
#include <string>

class Logger
{
public:
    static void log(const std::string& message);

private:
    static std::mutex logMutex;
};

#endif
