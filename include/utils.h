#ifndef XERXES_UTILS_H
#define XERXES_UTILS_H

#include "stdafx.h"

#include <arpa/inet.h>
#include <netdb.h>

namespace utils {

namespace randomizer {

int randomInt(int min, int max);

void randomIP(std::string &src);

int randomPort();

void randomstr(std::string &src);

void random_vec(std::vector<std::string> &vec, std::string &str);
} // namespace randomizer

namespace validator {

bool valid_host(const std::string &host);

bool valid_hostname(const std::string &hostname);

bool valid_port(const std::string &port);
} // namespace validator

void pause(timespec time);

void set_dly(int dly, timespec *time);

int to_int(const std::string &str);

unsigned short csum(unsigned short *buf, int len);

void daemonize();
}; // namespace utils

#endif // XERXES_UTILS_H
