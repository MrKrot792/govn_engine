#include "tve_log.hpp"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

Logger &Logger::getInstance()
{
    static Logger instance("log.txt");
    return instance;
}

void Logger::log(Level level, const std::string &message)
{
    std::lock_guard<std::mutex> lock(mutex_);

    std::ofstream file(logFile_, std::ios::app);
    if (!file)
    {
        std::cerr << "[ERROR] Can't open log file!\n";
        return;
    }

    std::string levelStr = getLevelString(level);
    std::string timestamp = getTimestamp();

    std::string logMessage = "[" + timestamp + "] [" + levelStr + "] " + message;

    std::cout << logMessage << std::endl; // Вывод в консоль
    file << logMessage << std::endl;      // Запись в файл
}

void Logger::vlog(Level level, const std::string &message, float value)
{
    std::lock_guard<std::mutex> lock(mutex_);

    std::ofstream file(logFile_, std::ios::app);
    if (!file)
    {
        std::cerr << "[ERROR] Can't open log file!\n";
        return;
    }

    std::string levelStr = getLevelString(level);
    std::string timestamp = getTimestamp();

    std::string logMessage = "[" + timestamp + "] [" + levelStr + "] " + message + ": " + std::to_string(value);

    std::cout << logMessage << std::endl; // Вывод в консоль
    file << logMessage << std::endl;      // Запись в файл
}

std::string Logger::getTimestamp()
{
    using namespace std::chrono;

    auto now = system_clock::now();
    auto now_time = system_clock::to_time_t(now);
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    std::ostringstream oss;
    oss << std::put_time(std::localtime(&now_time), "%Y-%m-%d %H:%M:%S") << '.' << std::setfill('0') << std::setw(3)
        << ms.count();

    return oss.str();
}

std::string Logger::getLevelString(Level level)
{
    switch (level)
    {
    case INFO:
        return "INFO";
    case WARNING:
        return "WARNING";
    case ERROR:
        return "ERROR";
    case DEBUG:
        return "DEBUG";
    default:
        return "UNKNOWN";
    }
}
