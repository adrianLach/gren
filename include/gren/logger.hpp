#pragma once
#include <iostream>
#include <string>
#include <chrono>

namespace gren
{
    class Logger
    {
    public:
        static void log(const std::string &message)
        {
            std::cout << getTimeStamp() << "[LOG] " << message << std::endl;
        }
        static void debug(const std::string &message)
        {
            std::cout << getTimeStamp() << "[DEBUG] " << message << std::endl;
        }

        static void error(const std::string &message)
        {
            std::cerr << getTimeStamp() << "[ERROR] " << message << std::endl;
        }

        static void warning(const std::string &message)
        {
            std::cerr << getTimeStamp() << "[WARNING] " << message << std::endl;
        }

    private:
        // Formatted as HH:MM:SS:mmm
        static std::string getTimeStamp()
        {
            auto now = std::chrono::system_clock::now();
            auto seconds = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
            auto hours = (seconds / 3600) % 24;
            auto minutes = (seconds / 60) % 60;
            auto secs = seconds % 60;
            auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000;

            return "[" + 
                   std::to_string(hours) + ":" + 
                   std::to_string(minutes) + ":" + 
                   std::to_string(secs) + ":" + 
                   std::to_string(milliseconds) + "] ";

        }
    };

}