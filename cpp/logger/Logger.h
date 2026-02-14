#include <iostream>
namespace CanvasMVP
{
    enum class LogLevel
    {
        Debug,
        Warn,
        Info,
        Error
    };
    class Logger
    {
    private:
        std::string tag;
        void log(LogLevel level, const std::string message);
        std::string levelToString(LogLevel level);

    public:
        Logger(std::string tag) : tag(tag) {};
        void debug(std::string message);
        void warn(std::string message);
        void info(std::string message);
        void error(std::string message);
    };
}