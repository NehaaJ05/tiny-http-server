#ifndef STATS_H
#define STATS_H

#include <atomic>
#include <chrono>
#include <string>

class Stats
{
public:

    static void initialize();

    static void requestStarted();

    static void requestFinished();

    static std::string getJson();

private:

    static std::atomic<int> activeConnections;

    static std::atomic<int> requestsServed;

    static std::chrono::steady_clock::time_point startTime;
};

#endif