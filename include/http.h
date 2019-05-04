#ifndef XERXES_HTTP_H
#define XERXES_HTTP_H

#include "stdafx.h"
#include "utils.h"

struct httphdr {
  std::string method{};
  std::string path{};
  std::string useragent{};
  std::string cache_control{};
  std::string encoding{};
  std::string charset[2]{};
  std::string referer{};
  std::string content_type{};
  std::string cookie[2]{};
  std::string connection_type{"Keep-Alive"};
  std::string accept{"*/*"};
};

namespace http {

namespace randomizer {

void random_referer(std::string &src);

void random_useragent(std::string &src);

void random_encoding(std::string &src);

void random_caching(std::string &src);

void random_charset(std::string &src);

void random_contenttype(std::string &src);

void random_method(std::string &src);
} // namespace randomizer

void hdr_gen(httphdr &hdr, std::string &str);

void load_usr(std::vector<std::string> &useragents);
} // namespace http

#endif // XERXES_HTTP_H
