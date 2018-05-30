#include "../Headers/httphdr.hpp"

#define TERMINATOR "\r\n"

void httphdr::setMethod(std::string &met) {
    method = met;
}

void httphdr::setLocation(std::string &loc) {
    location = loc;
}

void httphdr::setUseragent(std::string &usr) {
    useragent = usr;
}

void httphdr::setCacheControl(std::string &ctrl) {
    cache_control = ctrl;
}

void httphdr::setEncoding(std::string &enc) {
    encoding = enc;
}

void httphdr::setCharset(std::string &first, std::string &second) {
    charset[0] = first;
    charset[1] = second;
}

void httphdr::setReferer(std::string &ref) {
    referer = ref;
}

void httphdr::setContentType(std::string &type) {
    content_type = type;
}

void httphdr::setCookie(std::string &first, std::string &second) {
    cookie[0] = first;
    cookie[1] = second;
}

void httphdr::setKeepAlive(int keep) {
    keep_alive = keep;
}

void httphdr::setDNT(int D) {
    DNT = D;
}

void httphdr::setAccept(std::string &acc) {
    accept = acc;
}

void httphdr::setContentLength(int length) {
    content_length = length;
}

std::string httphdr::get_unterminated() {
    hdr = method + " /" + location + " HTTP/1.0" + TERMINATOR
             + "User-Agent: " + useragent + TERMINATOR
             + "Cache-Control: " + cache_control + TERMINATOR
             + "Accept-Encoding: " + encoding + TERMINATOR
             + "Accept-Charset: " + charset[0] + ", " + charset[1] + TERMINATOR
             + "Referer: " + referer + TERMINATOR
             + "Accept: " + accept + TERMINATOR
             + "Connection: " + connection_type + TERMINATOR
             + "Content-Type: " + content_type + TERMINATOR
             + (content_length ? "Content-Length: " + std::to_string(content_length) : "") + TERMINATOR
             + "Cookie: " + cookie[0] + "=" + cookie[1] + TERMINATOR
             + "Keep-Alive: " + std::to_string(keep_alive) + TERMINATOR
             + "DNT: " + std::to_string(DNT) + TERMINATOR;
    return hdr;
}

void httphdr::setConnectionType(std::string &type) {
    connection_type = type;
}

std::string httphdr::get() {
    return (get_unterminated() + TERMINATOR);
}
