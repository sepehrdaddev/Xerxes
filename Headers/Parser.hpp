#ifndef XERXES_PARSER_H
#define XERXES_PARSER_H

#include <string>
#include <memory>
#include <map>
#include <functional>

#include "Configuration.hpp"
#include "Engine.hpp"
#include "Validator.hpp"

class Parser {
public:
    void parse_commandline(int argc, const char *argv[]);
    Parser();
    explicit Parser(std::shared_ptr<Config> conf);
    static void help();
    static void show_banner();

private:
    std::shared_ptr<Config> conf;
    void check_root();
    void init_arguments();
    std::map<std::string, std::function<void(const char*)>> arguments;
};


#endif //XERXES_PARSER_H
