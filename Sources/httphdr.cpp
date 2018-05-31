#include "../Headers/httphdr.hpp"

#define TERMINATOR "\r\n"

void httphdr::generate_unterminated() {
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
}

void httphdr::generate() {
    generate_unterminated();
    hdr += TERMINATOR;
}

void httphdr::overide(std::string &header) {
    hdr = header;
}

std::string httphdr::get() {
    return hdr;
}

unsigned long httphdr::length() {
    return hdr.length();
}
