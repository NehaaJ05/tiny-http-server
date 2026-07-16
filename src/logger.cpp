#include "logger.h"

#include <iostream>
#include <chrono>
#include <iomanip>

std::mutex Logger::logMutex;

void Logger::log(const std::string& message)
{
    std::lock_guard<std::mutex> lock(logMutex);

    auto now = std::chrono::system_clock::now();

    auto time =std::chrono::system_clock::to_time_t(now);

    std::tm localTime = *std::localtime(&time);

    std::cout
        << std::put_time(&localTime,"%Y-%m-%d %H:%M:%S")
        << " | "
        << message
        << std::endl;
}