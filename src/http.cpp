#include "http.hpp"
#include "stdafx.hpp"

#include <algorithm>
#include <fstream>

namespace http {

namespace randomizer {

void random_referer(std::string &src) {
  static const std::vector<std::string> referer{
      "https://www.google.com/",   "https://www.yahoo.com/",
      "https://www.bing.com/",     "https://twitter.com/",
      "https://www.facebook.com/", "https://www.msn.com/",
      "https://www.youtube.com/",  "https://yandex.com/",
      "https://www.amazon.com/"};
  utils::randomizer::random_vec(referer, src);
}

void random_useragent(std::string &src) {
  static bool load = false;
  static std::vector<std::string> useragents{"Wget/1.16 (linux-gnu/Xerxes)"};
  if (!load) {
    http::load_usr(useragents);
    load = true;
  }
  utils::randomizer::random_vec(useragents, src);
}

void random_encoding(std::string &src) {
  static const std::vector<std::string> encoding{"\'\'", "*", "identity",
                                                 "gzip", "deflate"};
  utils::randomizer::random_vec(encoding, src);
}

void random_caching(std::string &src) {
  static const std::vector<std::string> caching{"no-cache", "max-age=0"};
  utils::randomizer::random_vec(caching, src);
}

void random_charset(std::string &src) {
  static const std::vector<std::string> charset{
      "ISO-8859-1", "utf-8", "Windows-1251", "ISO-8859-2", "ISO-8859-15"};
  utils::randomizer::random_vec(charset, src);
}

void random_contenttype(std::string &src) {
  static const std::vector<std::string> contenttype{
      "multipart/form-data", "application/x-url-encoded"};
  utils::randomizer::random_vec(contenttype, src);
}

void random_method(std::string &src) {
  static const std::vector<std::string> methods{"GET", "HEAD"};
  utils::randomizer::random_vec(methods, src);
}
} // namespace randomizer

void hdr_gen(httphdr &hdr, std::string &str) {
  str = fmt::format("{} /{} HTTP/1.1\r\nUser-Agent: {}\r\nCache-Control: "
                    "{}\r\nAccept-Encoding: {}\r\nAccept-Charset: {}, "
                    "{}\r\nReferer: {}\r\nAccept: {}\r\nConnection: "
                    "{}\r\nContent-Type: {}\r\nCookie: {}={}\r\n\r\n",
                    hdr.method, hdr.path, hdr.useragent, hdr.cache_control,
                    hdr.encoding, hdr.charset[0], hdr.charset[1], hdr.referer,
                    hdr.accept, hdr.connection_type, hdr.content_type,
                    hdr.cookie[0], hdr.cookie[1]);
}

void random_hdr(std::string &str) {}

void load_usr(std::vector<std::string> &useragents) {
  std::ifstream filestream("useragents");
  std::string line{};
  if (filestream.good() && filestream.is_open()) {
    long count = std::count(std::istreambuf_iterator<char>(filestream),
                            std::istreambuf_iterator<char>(), '\n');
    filestream.clear();
    filestream.seekg(0, std::ios::beg);
    useragents.reserve(static_cast<unsigned long>(count) + 1);
    while (getline(filestream, line))
      useragents.emplace_back(line);
    filestream.close();
  } else {
    spdlog::get("logger")->error("Unable to find useragents file");
  }
}
} // namespace http
