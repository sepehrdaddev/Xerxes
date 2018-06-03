#ifndef XERXES_HTTPHDR_H
#define XERXES_HTTPHDR_H


#include <string>
#include <array>

class httphdr {
public:
    httphdr& operator=(std::string& header) noexcept;
    httphdr& operator=(httphdr&& header) noexcept;
    httphdr();
    virtual ~httphdr();
    void generate();
    void generate_unterminated();
    const char *get();
    unsigned long length();

    std::string method{};
    std::string path{};
    std::string useragent{};
    std::string cache_control{};
    std::string encoding{};
    std::array<std::string, 2> charset{};
    std::string referer{};
    std::string content_type{};
    std::array<std::string, 2> cookie{};
    std::string connection_type{};
    int keep_alive = 0;
    int DNT = 0;
    std::string accept{};
    int content_length = 0;


private:
    std::string *hdr = nullptr;
};


#endif //XERXES_HTTPHDR_H
