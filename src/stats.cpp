#include "stats.h"

#include <sstream>

std::atomic<int> Stats::activeConnections = 0;
std::atomic<int> Stats::requestsServed = 0;

std::chrono::steady_clock::time_point Stats::startTime;

void Stats::initialize()
{
    startTime = std::chrono::steady_clock::now();
}

void Stats::requestStarted()
{
    activeConnections++;
    requestsServed++;
}

void Stats::requestFinished()
{
    activeConnections--;
}

std::string Stats::getJson()
{
    auto now = std::chrono::steady_clock::now();

    auto uptime =
        std::chrono::duration_cast<
            std::chrono::seconds>(
            now - startTime
        ).count();

    std::stringstream json;

    json
        << "{\n"
        << "  \"uptimeSeconds\": "
        << uptime
        << ",\n"

        << "  \"requestsServed\": "
        << requestsServed.load()
        << ",\n"

        << "  \"activeConnections\": "
        << activeConnections.load()
        << "\n}";

    return json.str();
}