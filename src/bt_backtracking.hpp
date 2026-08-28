// backtracking 
// El algoritmo construye la cadena de izquierda a derecha.
// En cada nodo se revisa si el prefijo todavia puede llegar
// a cumplir la politica. Si no puede, se poda la rama.

#pragma once

#include <string>
#include <chrono>
#include <stdexcept>
#include <algorithm>

namespace bt {

// El enunciado enumera 26 + 26 + 10 + 5 = 67 caracteres.
const std::string& get_alphabet() {
    static const std::string alphabet =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789"
        "!@#$%";

    return alphabet;
}
struct Policy {
    int minLower = 0;
    int minUpper = 0;
    int minDigit = 0;
    int minSymbol = 0;
    bool noConsecutive = true;
};

struct Result {
    long long nodesVisited = 0;
    long long nodesPruned = 0;
    long long solutions = 0;
    double timeMs = 0.0;
};

struct CompareResult {
    Result withPruning;
    Result withoutPruning;
    long long theoreticalNodes = 0;
    bool completeWithoutPruning = false;
};


// Clasificacion de caracteres

inline bool isLower(char c) {
    return c >= 'a' && c <= 'z';
}

inline bool isUpper(char c) {
    return c >= 'A' && c <= 'Z';
}

inline bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

inline bool isSymbol(char c) {
    return c == '!' || c == '@' || c == '#' ||
           c == '$' || c == '%';
}


// Comprueba una solucion completa


inline bool isValidSolution(const std::string& password,
                            const Policy& policy) {
    int lower = 0;
    int upper = 0;
    int digit = 0;
    int symbol = 0;

    for (size_t i = 0; i < password.size(); ++i) {
        char c = password[i];

        if (isLower(c)) {
            ++lower;
        } else if (isUpper(c)) {
            ++upper;
        } else if (isDigit(c)) {
            ++digit;
        } else if (isSymbol(c)) {
            ++symbol;
        }

        if (policy.noConsecutive &&
            i > 0 &&
            password[i] == password[i - 1]) {
            return false;
        }
    }

    return lower >= policy.minLower &&
           upper >= policy.minUpper &&
           digit >= policy.minDigit &&
           symbol >= policy.minSymbol;
}

// Cuenta los caracteres requeridos del prefijo.


inline void countTypes(const std::string& current,
                       int& lower,
                       int& upper,
                       int& digit,
                       int& symbol) {
    lower = upper = digit = symbol = 0;

    for (char c : current) {
        if (isLower(c)) {
            ++lower;
        } else if (isUpper(c)) {
            ++upper;
        } else if (isDigit(c)) {
            ++digit;
        } else if (isSymbol(c)) {
            ++symbol;
        }
    }
}
// Funcion de factibilidad


inline bool feasible(const std::string& current,
                     int remaining,
                     const Policy& policy) {
    int lower, upper, digit, symbol;
    countTypes(current, lower, upper, digit, symbol);

    int lowerNeeded = std::max(0, policy.minLower - lower);
    int upperNeeded = std::max(0, policy.minUpper - upper);
    int digitNeeded = std::max(0, policy.minDigit - digit);
    int symbolNeeded = std::max(0, policy.minSymbol - symbol);

    int totalNeeded =
        lowerNeeded + upperNeeded + digitNeeded + symbolNeeded;

    return totalNeeded <= remaining;
}

// Backtracking con poda


inline void backtrack(std::string& current,
                      int length,
                      const Policy& policy,
                      Result& result) {
    ++result.nodesVisited;

    // Caso base
    if (static_cast<int>(current.size()) == length) {
        if (isValidSolution(current, policy)) {
            ++result.solutions;
        }
        return;
    }

    int remaining =
        length - static_cast<int>(current.size()) - 1;

    for (char c : get_alphabet()) {
        // Poda 1
        if (policy.noConsecutive &&
            !current.empty() &&
            current.back() == c) {
            ++result.nodesPruned;
            continue;
        }

        current.push_back(c);

        // Poda 2
        if (!feasible(current, remaining, policy)) {
            ++result.nodesPruned;
            current.pop_back();
            continue;
        }

        backtrack(current, length, policy, result);

        // Backtracking: deshacer la decision.
        current.pop_back();
    }
}

// Backtracking con poda.


inline Result solve(const Policy& policy, int length) {
    if (length < 0) {
        throw std::invalid_argument("La longitud no puede ser negativa");
    }

    Result result;
    std::string current;
    current.reserve(static_cast<size_t>(length));

    auto start = std::chrono::steady_clock::now();

    if (feasible(current, length, policy)) {
        backtrack(current, length, policy, result);
    } else {
        // Se visita la raiz y se poda porque la instancia es imposible.
        result.nodesVisited = 1;
        result.nodesPruned = 1;
    }

    auto end = std::chrono::steady_clock::now();

    result.timeMs =
        std::chrono::duration<double, std::milli>(end - start).count();

    return result;
}


// Version sin poda


inline void exhaustive(std::string& current,
                       int length,
                       const Policy& policy,
                       Result& result) {
    ++result.nodesVisited;

    if (static_cast<int>(current.size()) == length) {
        if (isValidSolution(current, policy)) {
            ++result.solutions;
        }
        return;
    }

    for (char c : get_alphabet()) {
        current.push_back(c);
        exhaustive(current, length, policy, result);
        current.pop_back();
    }
}

inline Result solveWithoutPruning(const Policy& policy, int length) {
    if (length < 0) {
        throw std::invalid_argument("La longitud no puede ser negativa");
    }

    Result result;
    std::string current;
    current.reserve(static_cast<size_t>(length));

    auto start = std::chrono::steady_clock::now();

    exhaustive(current, length, policy, result);

    auto end = std::chrono::steady_clock::now();

    result.timeMs =
        std::chrono::duration<double, std::milli>(end - start).count();

    return result;
}


// Cantidad teorica de nodos del arbol completo:


inline long long theoreticalNodes(int length) {
    if (length < 0) {
        throw std::invalid_argument("La longitud no puede ser negativa");
    }

    // Para las longitudes de la practica, este calculo cabe en
    // unsigned long long. Se limita a LLONG_MAX si se desborda.
    unsigned long long total = 0;
    unsigned long long power = 1;
    const unsigned long long sigma = get_alphabet().size();

    for (int i = 0; i <= length; ++i) {
        if (total > 9223372036854775807ULL - power) {
            return 9223372036854775807LL;
        }

        total += power;

        if (i < length) {
            if (power > 9223372036854775807ULL / sigma) {
                return 9223372036854775807LL;
            }
            power *= sigma;
        }
    }

    return static_cast<long long>(total);
}

// Politicas solicitadas para los experimentos.
// La prohibicion de caracteres consecutivos se mantiene en todas
// las variantes, como indica el enunciado.


inline Policy teamPolicy() {
    Policy p;
    p.minLower = 2;
    p.minUpper = 1;
    p.minDigit = 1;
    p.minSymbol = 1;
    p.noConsecutive = true;
    return p;
}

inline Policy relaxedPolicy() {
    Policy p;
    p.minLower = 1;
    p.minUpper = 0;
    p.minDigit = 0;
    p.minSymbol = 0;
    p.noConsecutive = true;
    return p;
}

inline Policy noRestrictionsPolicy() {
    Policy p;
    p.minLower = 0;
    p.minUpper = 0;
    p.minDigit = 0;
    p.minSymbol = 0;
    p.noConsecutive = true;
    return p;
}

inline Policy policyByName(const std::string& name) {
    if (name == "team") {
        return teamPolicy();
    }

    if (name == "relaxed") {
        return relaxedPolicy();
    }

    if (name == "none") {
        return noRestrictionsPolicy();
    }

    throw std::invalid_argument(
        "Politica desconocida. Use team, relaxed o none."
    );
}


// Compara poda contra no poda.



inline CompareResult compare(const Policy& policy,
                             int length,
                             bool executeWithoutPruning = false) {
    CompareResult result;

    result.withPruning = solve(policy, length);
    result.theoreticalNodes = theoreticalNodes(length);

    if (executeWithoutPruning) {
        result.withoutPruning =
            solveWithoutPruning(policy, length);
        result.completeWithoutPruning = true;
    }

    return result;
}

} 
