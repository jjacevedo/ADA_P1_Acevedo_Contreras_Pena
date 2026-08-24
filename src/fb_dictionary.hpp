// fb_dictionary.hpp — ataque por diccionario: prueba solo lo que hay en resources/diccionario.txt.
#pragma once

#include <string>
#include <fstream>
#include <chrono>

#include "fb_hash.hpp"
#include "fb_bruteforce.hpp" // normalize_hex, SearchResult

namespace fb {

inline SearchResult dictionary_attack(const std::string& dict_path,
                                       const std::string& target_hash_hex) {
    SearchResult result;
    const std::string target = normalize_hex(target_hash_hex);

    std::ifstream in(dict_path);
    if (!in.is_open()) {
        throw std::runtime_error("No se pudo abrir el diccionario: " + dict_path);
    }

    const auto t0 = std::chrono::steady_clock::now();

    std::string candidate;
    while (std::getline(in, candidate)) {
        // Ignorar líneas vacías o terminadores \r residuales (archivos CRLF).
        while (!candidate.empty() && (candidate.back() == '\r' || candidate.back() == '\n')) {
            candidate.pop_back();
        }
        if (candidate.empty()) continue;

        ++result.candidates_evaluated;
        if (normalize_hex(sha256_hex(candidate)) == target) {
            result.found = true;
            result.password = candidate;
            break;
        }
    }

    const auto t1 = std::chrono::steady_clock::now();
    result.time_ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
    return result;
}

} // namespace fb
