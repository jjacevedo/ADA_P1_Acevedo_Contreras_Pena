// main.cpp - CLI del proyecto FB + BT.
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <stdexcept>

#include "fb_hash.hpp"
#include "fb_alphabet.hpp"
#include "fb_bruteforce.hpp"
#include "fb_dictionary.hpp"
#include "fb_seed.hpp"
#include "bt_backtracking.hpp"

namespace {

const std::string REFERENCE_HASH =
    "8d51feb34e3e69f6fa6dffc577e2c60490cf9a7fcd835f9f6af1505b71d74773";
const std::string REFERENCE_PASSWORD = "abc12";
const int REFERENCE_LENGTH = 5;

std::map<std::string, std::string> parse_flags(
    int argc, char** argv, int start) {

    std::map<std::string, std::string> flags;

    for (int i = start; i + 1 < argc; i += 2) {
        std::string key = argv[i];

        if (key.rfind("--", 0) == 0) {
            key = key.substr(2);
        }

        flags[key] = argv[i + 1];
    }

    return flags;
}

std::vector<std::string> split_comma(const std::string& s) {
    std::vector<std::string> parts;
    std::stringstream ss(s);
    std::string item;

    while (std::getline(ss, item, ',')) {
        size_t a = item.find_first_not_of(' ');
        size_t b = item.find_last_not_of(' ');

        if (a == std::string::npos) {
            continue;
        }

        parts.push_back(item.substr(a, b - a + 1));
    }

    return parts;
}

void print_usage() {
    std::cout <<
        "Uso:\n"
        "  ada_p1_fb hash <texto>\n"
        "  ada_p1_fb verify-ref\n"
        "  ada_p1_fb search --alphabet a1|a2 --length N --hash <hex>\n"
        "  ada_p1_fb dict --hash <hex> [--dict resources/diccionario.txt]\n"
        "  ada_p1_fb gen-instances --apellidos \"ape1,ape2,ape3\"\n"
        "  ada_p1_fb bt --length N [--policy team|relaxed|none] [--pruning on|off]\n"
        "  ada_p1_fb bt-compare --length N [--policy team|relaxed|none] [--run-no-pruning 0|1]\n";
}

// ------------------------------------------------------------
// Comandos FB existentes.
// ------------------------------------------------------------

int cmd_hash(int argc, char** argv) {
    if (argc < 3) {
        print_usage();
        return 1;
    }

    std::cout << fb::sha256_hex(argv[2]) << "\n";
    return 0;
}

int cmd_verify_ref() {
    std::cout << "Instancia de referencia: \"" << REFERENCE_PASSWORD
              << "\" sobre A2, n=" << REFERENCE_LENGTH << "\n";
    std::cout << "Hash objetivo: " << REFERENCE_HASH << "\n";

    auto r = fb::exhaustive_search(
        fb::ALPHABET_A2,
        REFERENCE_LENGTH,
        REFERENCE_HASH
    );

    std::cout << "Encontrada: "
              << (r.found ? "SI" : "NO") << "\n";

    std::cout << "Contraseña recuperada: "
              << r.password << "\n";

    std::cout << "Candidatos evaluados: "
              << r.candidates_evaluated << "\n";

    std::cout << "Tiempo (ms): "
              << r.time_ms << "\n";

    bool ok =
        r.found &&
        r.password == REFERENCE_PASSWORD;

    std::cout <<
        (ok
            ? "[OK] Implementacion validada correctamente.\n"
            : "[ERROR] La implementacion NO reproduce la instancia de referencia.\n");

    return ok ? 0 : 1;
}

int cmd_search(int argc, char** argv) {
    auto flags = parse_flags(argc, argv, 2);

    if (!flags.count("alphabet") ||
        !flags.count("length") ||
        !flags.count("hash")) {

        std::cerr
            << "Faltan argumentos. Uso: "
            << "search --alphabet a1|a2 --length N --hash <hex>\n";

        return 1;
    }

    const std::string& alphabet =
        fb::alphabet_by_name(flags["alphabet"]);

    int length =
        std::stoi(flags["length"]);

    const std::string& hash =
        flags["hash"];

    auto r =
        fb::exhaustive_search(
            alphabet,
            length,
            hash
        );

    std::cout
        << "alfabeto=" << flags["alphabet"]
        << " |sigma|=" << alphabet.size()
        << " n=" << length << "\n";

    std::cout
        << "encontrada="
        << (r.found ? 1 : 0)
        << "\n";

    if (r.found) {
        std::cout
            << "password="
            << r.password
            << "\n";
    }

    std::cout
        << "candidatos_evaluados="
        << r.candidates_evaluated
        << "\n";

    std::cout
        << "tiempo_ms="
        << r.time_ms
        << "\n";

    return 0;
}

int cmd_dict(int argc, char** argv) {
    auto flags = parse_flags(argc, argv, 2);

    if (!flags.count("hash")) {
        std::cerr
            << "Faltan argumentos. Uso: "
            << "dict --hash <hex> [--dict path]\n";
        return 1;
    }

    std::string dict_path =
        flags.count("dict")
            ? flags["dict"]
            : "resources/diccionario.txt";

    auto r =
        fb::dictionary_attack(
            dict_path,
            flags["hash"]
        );

    std::cout
        << "diccionario="
        << dict_path
        << "\n";

    std::cout
        << "encontrada="
        << (r.found ? 1 : 0)
        << "\n";

    if (r.found) {
        std::cout
            << "password="
            << r.password
            << "\n";
    }

    std::cout
        << "candidatos_evaluados="
        << r.candidates_evaluated
        << "\n";

    std::cout
        << "tiempo_ms="
        << r.time_ms
        << "\n";

    return 0;
}

int cmd_gen_instances(int argc, char** argv) {
    auto flags = parse_flags(argc, argv, 2);

    if (!flags.count("apellidos")) {
        std::cerr
            << "Uso: gen-instances "
            << "--apellidos \"ape1,ape2,ape3\"\n";
        return 1;
    }

    std::vector<std::string> apellidos =
        split_comma(flags["apellidos"]);

    std::vector<std::string> normalizados;

    for (auto& a : apellidos) {
        normalizados.push_back(
            fb::normalize_apellido(a)
        );
    }

    std::vector<std::string> ordenados =
        normalizados;

    std::sort(
        ordenados.begin(),
        ordenados.end()
    );

    if (ordenados != normalizados) {
        std::cerr
            << "[ADVERTENCIA] Los apellidos no estan en orden alfabetico.\n";

        return 1;
    }

    long long seed =
        fb::compute_team_seed(apellidos);

    std::cout
        << "Apellidos: ";

    for (auto& a : normalizados) {
        std::cout << a << " ";
    }

    std::cout << "\nSemilla del equipo = "
              << seed
              << "\n\n";

    std::vector<int> lengths =
        {4, 4, 5, 5, 6};

    std::vector<std::string> alph_names =
        {"a1", "a2", "a1", "a2", "a1"};

    fb::LCG lcg(seed);

    std::cout
        << "n | alfabeto | contraseña | sha256\n";

    std::cout
        << "--------------------------------------------------------------\n";

    for (size_t i = 0; i < lengths.size(); ++i) {

        const std::string& alphabet =
            fb::alphabet_by_name(
                alph_names[i]
            );

        std::string pwd =
            fb::generate_password(
                lcg,
                alphabet,
                lengths[i]
            );

        std::string h =
            fb::sha256_hex(pwd);

        std::cout
            << lengths[i]
            << " | "
            << alph_names[i]
            << " | "
            << pwd
            << " | "
            << h
            << "\n";
    }

    return 0;
}

// ------------------------------------------------------------
// Comandos BT.
// ------------------------------------------------------------

bt::Policy get_bt_policy(
    const std::map<std::string, std::string>& flags) {

    if (!flags.count("policy")) {
        return bt::teamPolicy();
    }

    return bt::policyByName(
        flags.at("policy")
    );
}

void print_policy(const bt::Policy& p) {
    std::cout
        << "minLower=" << p.minLower << "\n"
        << "minUpper=" << p.minUpper << "\n"
        << "minDigit=" << p.minDigit << "\n"
        << "minSymbol=" << p.minSymbol << "\n"
        << "noConsecutive="
        << (p.noConsecutive ? 1 : 0)
        << "\n";
}

int cmd_bt(int argc, char** argv) {
    auto flags =
        parse_flags(argc, argv, 2);

    if (!flags.count("length")) {
        std::cerr
            << "Uso: bt --length N "
            << "[--policy team|relaxed|none] "
            << "[--pruning on|off]\n";
        return 1;
    }

    int length =
        std::stoi(flags["length"]);

    bt::Policy policy;

    try {
        policy = get_bt_policy(flags);
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }

    bool pruning = true;

    if (flags.count("pruning")) {
        pruning =
            flags["pruning"] != "off";
    }

    bt::Result result;

    try {
        if (pruning) {
            result =
                bt::solve(
                    policy,
                    length
                );
        } else {
            result =
                bt::solveWithoutPruning(
                    policy,
                    length
                );
        }
    } catch (const std::exception& e) {
        std::cerr
            << "Error: "
            << e.what()
            << "\n";
        return 1;
    }

    std::cout
        << "modulo=BT\n"
        << "alfabeto=BT\n"
        << "sigma=" << bt::get_alphabet().size()
        << "\n"
        << "n=" << length
        << "\n"
        << "politica="
        << (flags.count("policy")
            ? flags["policy"]
            : "team")
        << "\n"
        << "poda="
        << (pruning ? 1 : 0)
        << "\n";

    print_policy(policy);

    std::cout
        << "nodos_visitados="
        << result.nodesVisited
        << "\n"
        << "nodos_podados="
        << result.nodesPruned
        << "\n"
        << "soluciones="
        << result.solutions
        << "\n"
        << "tiempo_ms="
        << result.timeMs
        << "\n";

    return 0;
}

int cmd_bt_compare(int argc, char** argv) {
    auto flags =
        parse_flags(argc, argv, 2);

    if (!flags.count("length")) {
        std::cerr
            << "Uso: bt-compare --length N "
            << "[--policy team|relaxed|none] "
            << "[--run-no-pruning 0|1]\n";
        return 1;
    }

    int length =
        std::stoi(flags["length"]);

    bt::Policy policy;

    try {
        policy = get_bt_policy(flags);
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }

    bool runWithoutPruning = false;

    if (flags.count("run-no-pruning")) {
        runWithoutPruning =
            flags["run-no-pruning"] == "1";
    }

    bt::CompareResult result;

    try {
        result =
            bt::compare(
                policy,
                length,
                runWithoutPruning
            );
    } catch (const std::exception& e) {
        std::cerr
            << "Error: "
            << e.what()
            << "\n";
        return 1;
    }

    std::cout
        << "modulo=BT\n"
        << "sigma=" << bt::get_alphabet().size()    
        << "\n"
        << "n=" << length
        << "\n"
        << "politica="
        << (flags.count("policy")
            ? flags["policy"]
            : "team")
        << "\n"
        << "nodos_arbol_completo="
        << result.theoreticalNodes
        << "\n"
        << "nodos_con_poda="
        << result.withPruning.nodesVisited
        << "\n"
        << "nodos_podados="
        << result.withPruning.nodesPruned
        << "\n"
        << "soluciones_con_poda="
        << result.withPruning.solutions
        << "\n"
        << "tiempo_con_poda_ms="
        << result.withPruning.timeMs
        << "\n";

    if (result.theoreticalNodes > 0) {
        double reduction =
            100.0 *
            (1.0 -
             static_cast<double>(
                 result.withPruning.nodesVisited
             ) /
             static_cast<double>(
                 result.theoreticalNodes
             ));

        std::cout
            << "reduccion_nodos_porcentaje="
            << reduction
            << "\n";
    }

    if (result.completeWithoutPruning) {
        std::cout
            << "nodos_sin_poda="
            << result.withoutPruning.nodesVisited
            << "\n"
            << "soluciones_sin_poda="
            << result.withoutPruning.solutions
            << "\n"
            << "tiempo_sin_poda_ms="
            << result.withoutPruning.timeMs
            << "\n"
            << "soluciones_iguales="
            << (result.withoutPruning.solutions ==
                result.withPruning.solutions ? 1 : 0)
            << "\n";
    } else {
        std::cout
            << "sin_poda_ejecutada=0\n"
            << "nota=Use --run-no-pruning 1 solo en instancias pequenas.\n";
    }

    return 0;
}

} // namespace

int main(int argc, char** argv) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    std::string cmd =
        argv[1];

    try {
        if (cmd == "hash")
            return cmd_hash(argc, argv);

        if (cmd == "verify-ref")
            return cmd_verify_ref();

        if (cmd == "search")
            return cmd_search(argc, argv);

        if (cmd == "dict")
            return cmd_dict(argc, argv);

        if (cmd == "gen-instances")
            return cmd_gen_instances(argc, argv);

        if (cmd == "bt")
            return cmd_bt(argc, argv);

        if (cmd == "bt-compare")
            return cmd_bt_compare(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] " << e.what() << "\n";
        return 1;
    }

    print_usage();
    return 1;
}
