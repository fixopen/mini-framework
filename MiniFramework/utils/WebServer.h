#pragma once

#include <string> // for std::string

namespace utils {
    class WebServer {
    public:
        WebServer(std::string const& host, unsigned short port, std::string const& root);
    private:
    };
}