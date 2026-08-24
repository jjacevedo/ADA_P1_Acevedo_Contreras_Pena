// fb_hash.hpp — wrapper sobre picosha2 para sha256 en hex.
#pragma once

#include <string>
#include "third_party/picosha2.h"

namespace fb {

inline std::string sha256_hex(const std::string& input) {
    return picosha2::hash256_hex_string(input);
}

} // namespace fb
