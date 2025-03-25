#include <ctime>
#include <fstream>
#include <mutex>

#define LOG_INFO(msg) Logger::getInstance().log(Logger::INFO, msg)
#define LOG_WARN(msg) Logger::getInstance().log(Logger::WARNING, msg)
#define LOG_ERROR(msg) Logger::getInstance().log(Logger::ERROR, msg)
#define LOG_DEBUG(msg) Logger::getInstance().log(Logger::DEBUG, msg)

#define VLOG_INFO(msg, vl) Logger::getInstance().log(Logger::INFO, msg, vl)
#define VLOG_WARN(msg, vl) Logger::getInstance().log(Logger::WARNING, msg, vl)
#define VLOG_ERROR(msg, vl) Logger::getInstance().log(Logger::ERROR, msg, vl)
#define VLOG_DEBUG(msg, vl) Logger::getInstance().log(Logger::DEBUG, msg, vl)

class Logger
{
  public:
    enum Level
    {
        INFO,
        WARNING,
        ERROR,
        DEBUG
    };

    static Logger &getInstance();

    void log(Level level, const std::string &message);
    void vlog(Level level, const std::string &message, float value);

  private:
    std::string logFile_;
    std::mutex mutex_;

    Logger(const std::string &filename) : logFile_(filename)
    {
    }

    std::string getTimestamp();

    std::string getLevelString(Level level);
};
