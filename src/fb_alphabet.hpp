// fb_alphabet.hpp — alfabetos A1 y A2 del proyecto.
#pragma once
#include <string>
#include <stdexcept>
#include <cctype>

namespace fb {

// static evita las inline variables, que no son soportadas por GCC 6.3.
static const std::string ALPHABET_A1 = "abcdefghijklmnopqrstuvwxyz";
static const std::string ALPHABET_A2 = "abcdefghijklmnopqrstuvwxyz0123456789";

inline const std::string& alphabet_by_name(const std::string& name) {
    std::string lower;
    for (char c : name) {
        lower += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }

    if (lower == "a1") return ALPHABET_A1;
    if (lower == "a2") return ALPHABET_A2;

    throw std::invalid_argument(
        "Alfabeto desconocido: '" + name + "' (use 'a1' o 'a2')"
    );
}

} // namespace fb
