#include "Logger.h"

namespace CanvasMVP
{

    void Logger::debug(std::string message)
    {
        log(LogLevel::Debug, message);
    }

    void Logger::error(std::string message)
    {
        log(LogLevel::Error, message);
    }
    void Logger::warn(std::string message)
    {
        log(LogLevel::Warn, message);
    }
    void Logger::info(std::string message)
    {
        log(LogLevel::Info, message);
    }
    void Logger::log(LogLevel level, const std::string message)
    {
        std::cout
            << "[" << levelToString(level) << "]"
            << "[" << tag << "] "
            << message << std::endl;
    }

    std::string Logger::levelToString(LogLevel level)
    {
        switch (level)
        {
        case LogLevel::Debug:
            return "DEBUG";
        case LogLevel::Warn:
            return "Warn";
        case LogLevel::Info:
            return "Info";
        case LogLevel::Error:
            return "Error";
        default:
            return "Unknown";
        }
    }
}