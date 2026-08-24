// fb_seed.hpp — semilla del equipo (apellidos) + generador LCG de instancias.
#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <cctype>

namespace fb {

inline std::string normalize_apellido(const std::string& raw) {
    static const std::vector<std::pair<std::string, char>> accents = {
        {"á", 'a'}, {"é", 'e'}, {"í", 'i'}, {"ó", 'o'}, {"ú", 'u'},
        {"Á", 'a'}, {"É", 'e'}, {"Í", 'i'}, {"Ó", 'o'}, {"Ú", 'u'},
        {"ñ", 'n'}, {"Ñ", 'n'}, {"ü", 'u'}, {"Ü", 'u'}
    };
    std::string s = raw;
    for (const auto& [pat, rep] : accents) {
        size_t pos;
        while ((pos = s.find(pat)) != std::string::npos) {
            s.replace(pos, pat.size(), std::string(1, rep));
        }
    }
    std::string out;
    for (char c : s) {
        if (std::isalpha(static_cast<unsigned char>(c))) {
            out += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        }
        // cualquier otro caracter (espacio, guión...) se descarta: "sin espacios".
    }
    return out;
}

// apellidos_ordenados: ya en orden alfabético.
inline long long compute_team_seed(const std::vector<std::string>& apellidos_ordenados) {
    std::string concat;
    for (const auto& ap : apellidos_ordenados) {
        concat += normalize_apellido(ap);
    }
    long long sum = 0;
    for (unsigned char c : concat) sum += static_cast<long long>(c);
    return sum % 100000;
}

// No se reinicia entre contraseñas: el estado sigue de una a la siguiente.
class LCG {
public:
    explicit LCG(long long seed) : x_(seed) {}

    long long next() {
        long long current = x_;
        x_ = (1103515245LL * x_ + 12345LL) % (1LL << 31);
        return current;
    }

private:
    long long x_;
};

inline std::string generate_password(LCG& lcg, const std::string& alphabet, int length) {
    std::string pwd;
    pwd.reserve(length);
    const long long sigma = static_cast<long long>(alphabet.size());
    for (int i = 0; i < length; ++i) {
        long long xi = lcg.next();
        pwd += alphabet[xi % sigma];
    }
    return pwd;
}

} // namespace fb
