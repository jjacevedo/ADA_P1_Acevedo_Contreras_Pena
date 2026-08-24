// fb_bruteforce.hpp — fuerza bruta: recorre Σ^n con un odómetro de base |Σ|.
#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <cstdint>
#include <algorithm>
#include <cctype>

#include "fb_hash.hpp"

namespace fb {

struct SearchResult {
    bool found = false;
    std::string password;              // solo válido si found == true
    long long candidates_evaluated = 0;
    double time_ms = 0.0;
};

inline std::string normalize_hex(const std::string& h) {
    std::string out = h;
    std::transform(out.begin(), out.end(), out.begin(),
                    [](unsigned char c) { return std::tolower(c); });
    return out;
}

inline SearchResult exhaustive_search(const std::string& alphabet,
                                       int length,
                                       const std::string& target_hash_hex) {
    SearchResult result;
    const std::string target = normalize_hex(target_hash_hex);
    const int sigma = static_cast<int>(alphabet.size());

    // Odómetro: un índice por posición, todos en 0 al inicio (=> alphabet[0]...alphabet[0]).
    std::vector<int> idx(length, 0);
    std::string candidate(length, alphabet[0]);

    const auto t0 = std::chrono::steady_clock::now();

    while (true) {
        // Construir el candidato actual a partir de los índices del odómetro.
        for (int p = 0; p < length; ++p) candidate[p] = alphabet[idx[p]];

        ++result.candidates_evaluated;
        if (normalize_hex(sha256_hex(candidate)) == target) {
            result.found = true;
            result.password = candidate;
            break;
        }

        // Incrementar el odómetro (posición más a la derecha primero, con acarreo).
        int pos = length - 1;
        while (pos >= 0) {
            idx[pos]++;
            if (idx[pos] < sigma) break;   // sin acarreo, listo
            idx[pos] = 0;                  // desbordó: reinicia y propaga acarreo
            --pos;
        }
        if (pos < 0) break;                // desbordó la posición 0 => espacio agotado
    }

    const auto t1 = std::chrono::steady_clock::now();
    result.time_ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
    return result;
}

} // namespace fb
