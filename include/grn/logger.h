#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace grn
{
    class Logger
    {
    public:
        // ANSI color codes
        static constexpr const char *RESET = "\033[0m";
        static constexpr const char *WHITE = "\033[37m";
        static constexpr const char *CYAN = "\033[36m";
        static constexpr const char *RED = "\033[31m";
        static constexpr const char *YELLOW = "\033[33m";

        static void setDebugEnabled(bool enabled)
        {
            s_debugEnabled = enabled;
        }

        static void log(const std::string &message)
        {
            std::cout << getTimeStamp() << WHITE << "[LOG]     " << message << RESET << std::endl;
        }
        static void debug(const std::string &message)
        {
            if (!isDebugEnabled())
                return;
            std::cout << getTimeStamp() << CYAN << "[DEBUG]   " << message << RESET << std::endl;
        }

        static void error(const std::string &message)
        {
            std::cerr << getTimeStamp() << RED << "[ERROR]   " << message << RESET << std::endl;
        }

        static void warning(const std::string &message)
        {
            std::cerr << getTimeStamp() << YELLOW << "[WARNING] " << message << RESET << std::endl;
        }

    private:
        static inline bool s_debugEnabled = false;
        static bool isDebugEnabled()
        {
            return s_debugEnabled;
        }
        // Formatted as HH:MM:SS:mmmm (4-digit milliseconds)
        static std::string getTimeStamp()
        {
            auto now = std::chrono::system_clock::now();
            auto seconds = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
            auto hours = (seconds / 3600) % 24;
            auto minutes = (seconds / 60) % 60;
            auto secs = seconds % 60;
            auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 10000;

            std::stringstream ss;
            ss << "["
               << std::setfill('0') << std::setw(2) << hours << ":"
               << std::setfill('0') << std::setw(2) << minutes << ":"
               << std::setfill('0') << std::setw(2) << secs << ":"
               << std::setfill('0') << std::setw(4) << milliseconds << "] ";

            return ss.str();
        }
    };

}