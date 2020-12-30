#pragma once

#include <string>

namespace utils::base64 {
    std::string const encode(std::string const &data);

    std::string const decode(std::string const &data);
}

