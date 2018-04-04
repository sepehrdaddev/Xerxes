#include <iostream>
#include "Logger.hpp"

#define RED   "\x1B[31m" // Color Terminal Red
#define YEL   "\x1B[33m" // Color Terminal Yellow
#define GRN   "\x1B[32m" // Color Terminal Green
#define RESET "\x1B[0m" // Color Terminal Reset color

void Logger::Log(std::string *message, Level l) {
    if(level >= l){
        switch (l){
            case Level::None:
                break;
            case Level::Error:
                std::cerr << RED << "[ERROR]: " << *message << RESET << "\n";
                break;
            case Level::Warning:
                std::cout << YEL << "[WARNING]: " << *message << RESET << "\n";
                break;
            case Level::Info:
                std::cout << GRN << "[INFO]: " << *message << RESET << "\n";
                break;
            default:
                return;
        }
    }
}

void Logger::Log(const char *message, Logger::Level l) {
    std::string str = std::string{message};
    Log(&str, l);
}

Logger::Logger() = default;

Logger::Logger(Level l): level{l}{

}

void Logger::setLevel(Logger::Level l) {
    level = l;
}