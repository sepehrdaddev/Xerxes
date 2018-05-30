#ifndef XERXES_HTTPHDR_H
#define XERXES_HTTPHDR_H


#include <string>
#include <array>

class httphdr {
public:
    std::string get();
    std::string get_unterminated();
    void setMethod(std::string& met);
    void setLocation(std::string& loc);
    void setUseragent(std::string& usr);
    void setCacheControl(std::string& ctrl);
    void setEncoding(std::string& enc);
    void setCharset(std::string& first, std::string& second);
    void setReferer(std::string& ref);
    void setConnectionType(std::string& type);
    void setContentType(std::string& type);
    void setCookie(std::string& first, std::string& second);
    void setKeepAlive(int keep);
    void setDNT(int D);
    void setAccept(std::string& acc);
    void setContentLength(int length);

private:
    std::string method{};
    std::string location{};
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
    std::string hdr{};
};


#endif //XERXES_HTTPHDR_H
