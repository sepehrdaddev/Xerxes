#ifndef XERXES_LOGGER_H
#define XERXES_LOGGER_H

class Logger {
public:
    enum Level{None, Error, Warning, Info};
    Logger();
    explicit Logger(Level l);

    void Log(std::string *message, Level l);
    void Log(const char* message, Level l);
    void setLevel(Level l);

private:
    Level level{Warning};
};


#endif //XERXES_LOGGER_H
