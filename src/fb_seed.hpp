// fb_seed.hpp — semilla del equipo (apellidos) + generador LCG de instancias.
#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <cctype>

namespace fb {

inline std::string normalize_apellido(const std::string& raw) {
    static const std::vector<std::pair<std::string, char> > accents = {
        {"á", 'a'}, {"é", 'e'}, {"í", 'i'}, {"ó", 'o'}, {"ú", 'u'},
        {"Á", 'a'}, {"É", 'e'}, {"Í", 'i'}, {"Ó", 'o'}, {"Ú", 'u'},
        {"ñ", 'n'}, {"Ñ", 'n'}, {"ü", 'u'}, {"Ü", 'u'}
    };

    std::string s = raw;

    // Forma compatible con GCC 6.3: no usa structured bindings.
    for (size_t i = 0; i < accents.size(); ++i) {
        const std::string& pat = accents[i].first;
        char rep = accents[i].second;
        size_t pos;

        while ((pos = s.find(pat)) != std::string::npos) {
            s.replace(pos, pat.size(), std::string(1, rep));
        }
    }

    std::string out;

    for (size_t i = 0; i < s.size(); ++i) {
        unsigned char uc = static_cast<unsigned char>(s[i]);

        if (std::isalpha(uc)) {
            out += static_cast<char>(std::tolower(uc));
        }
        // Espacios, guiones y otros caracteres se descartan.
    }

    return out;
}

// apellidos_ordenados: ya en orden alfabético.
inline long long compute_team_seed(
    const std::vector<std::string>& apellidos_ordenados) {

    std::string concat;

    for (size_t i = 0; i < apellidos_ordenados.size(); ++i) {
        concat += normalize_apellido(apellidos_ordenados[i]);
    }

    long long sum = 0;

    for (size_t i = 0; i < concat.size(); ++i) {
        unsigned char c = static_cast<unsigned char>(concat[i]);
        sum += static_cast<long long>(c);
    }

    return sum % 100000;
}

// No se reinicia entre contraseñas.
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

inline std::string generate_password(
    LCG& lcg,
    const std::string& alphabet,
    int length) {

    std::string pwd;
    pwd.reserve(static_cast<size_t>(length));

    const long long sigma = static_cast<long long>(alphabet.size());

    for (int i = 0; i < length; ++i) {
        long long xi = lcg.next();
        pwd += alphabet[xi % sigma];
    }

    return pwd;
}

} // namespace fb
