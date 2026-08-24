// main.cpp — CLI del Módulo FB (uso y compilación: ver README).

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include "fb_hash.hpp"
#include "fb_alphabet.hpp"
#include "fb_bruteforce.hpp"
#include "fb_dictionary.hpp"
#include "fb_seed.hpp"

namespace {

// Instancia de referencia común a todo el curso, usada por verify-ref.
const std::string REFERENCE_HASH =
    "8d51feb34e3e69f6fa6dffc577e2c60490cf9a7fcd835f9f6af1505b71d74773";
const std::string REFERENCE_PASSWORD = "abc12";
const int REFERENCE_LENGTH = 5; // alfabeto A2

// Parser de argumentos --clave valor muy simple (suficiente para esta CLI).
std::map<std::string, std::string> parse_flags(int argc, char** argv, int start) {
    std::map<std::string, std::string> flags;
    for (int i = start; i + 1 < argc; i += 2) {
        std::string key = argv[i];
        if (key.rfind("--", 0) == 0) key = key.substr(2);
        flags[key] = argv[i + 1];
    }
    return flags;
}

std::vector<std::string> split_comma(const std::string& s) {
    std::vector<std::string> parts;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, ',')) {
        // recorta espacios en los extremos
        size_t a = item.find_first_not_of(' ');
        size_t b = item.find_last_not_of(' ');
        if (a == std::string::npos) continue;
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
        "  ada_p1_fb gen-instances --apellidos \"ape1,ape2,ape3\"\n";
}

int cmd_hash(int argc, char** argv) {
    if (argc < 3) { print_usage(); return 1; }
    std::cout << fb::sha256_hex(argv[2]) << "\n";
    return 0;
}

int cmd_verify_ref() {
    std::cout << "Instancia de referencia: \"" << REFERENCE_PASSWORD
              << "\" sobre A2, n=" << REFERENCE_LENGTH << "\n";
    std::cout << "Hash objetivo: " << REFERENCE_HASH << "\n";
    auto r = fb::exhaustive_search(fb::ALPHABET_A2, REFERENCE_LENGTH, REFERENCE_HASH);
    std::cout << "Encontrada: " << (r.found ? "SI" : "NO") << "\n";
    std::cout << "Contraseña recuperada: " << r.password << "\n";
    std::cout << "Candidatos evaluados: " << r.candidates_evaluated << "\n";
    std::cout << "Tiempo (ms): " << r.time_ms << "\n";
    bool ok = r.found && r.password == REFERENCE_PASSWORD;
    std::cout << (ok ? "[OK] Implementacion validada correctamente.\n"
                      : "[ERROR] La implementacion NO reproduce la instancia de referencia.\n");
    return ok ? 0 : 1;
}

int cmd_search(int argc, char** argv) {
    auto flags = parse_flags(argc, argv, 2);
    if (!flags.count("alphabet") || !flags.count("length") || !flags.count("hash")) {
        std::cerr << "Faltan argumentos. Uso: search --alphabet a1|a2 --length N --hash <hex>\n";
        return 1;
    }
    const std::string& alphabet = fb::alphabet_by_name(flags["alphabet"]);
    int length = std::stoi(flags["length"]);
    const std::string& hash = flags["hash"];

    auto r = fb::exhaustive_search(alphabet, length, hash);
    std::cout << "alfabeto=" << flags["alphabet"] << " |sigma|=" << alphabet.size()
              << " n=" << length << "\n";
    std::cout << "encontrada=" << (r.found ? 1 : 0) << "\n";
    if (r.found) std::cout << "password=" << r.password << "\n";
    std::cout << "candidatos_evaluados=" << r.candidates_evaluated << "\n";
    std::cout << "tiempo_ms=" << r.time_ms << "\n";
    return 0;
}

int cmd_dict(int argc, char** argv) {
    auto flags = parse_flags(argc, argv, 2);
    if (!flags.count("hash")) {
        std::cerr << "Faltan argumentos. Uso: dict --hash <hex> [--dict path]\n";
        return 1;
    }
    std::string dict_path = flags.count("dict") ? flags["dict"] : "resources/diccionario.txt";
    auto r = fb::dictionary_attack(dict_path, flags["hash"]);
    std::cout << "diccionario=" << dict_path << "\n";
    std::cout << "encontrada=" << (r.found ? 1 : 0) << "\n";
    if (r.found) std::cout << "password=" << r.password << "\n";
    std::cout << "candidatos_evaluados=" << r.candidates_evaluated << "\n";
    std::cout << "tiempo_ms=" << r.time_ms << "\n";
    return 0;
}

int cmd_gen_instances(int argc, char** argv) {
    auto flags = parse_flags(argc, argv, 2);
    if (!flags.count("apellidos")) {
        std::cerr << "Uso: gen-instances --apellidos \"ape1,ape2,ape3\" (ya en orden alfabetico)\n";
        return 1;
    }
    std::vector<std::string> apellidos = split_comma(flags["apellidos"]);
    // Verificación defensiva: deben quedar en orden alfabético (el enunciado lo exige).
    std::vector<std::string> normalizados;
    for (auto& a : apellidos) normalizados.push_back(fb::normalize_apellido(a));
    std::vector<std::string> ordenados = normalizados;
    std::sort(ordenados.begin(), ordenados.end());
    if (ordenados != normalizados) {
        std::cerr << "[ADVERTENCIA] Los apellidos no estan en orden alfabetico. "
                     "El enunciado exige ordenarlos alfabeticamente antes de concatenar.\n"
                     "Orden recibido:  ";
        for (auto& a : normalizados) std::cerr << a << " ";
        std::cerr << "\nOrden correcto: ";
        for (auto& a : ordenados) std::cerr << a << " ";
        std::cerr << "\n";
        return 1;
    }

    long long seed = fb::compute_team_seed(apellidos);
    std::cout << "Apellidos (normalizados, orden alfabetico): ";
    for (auto& a : normalizados) std::cout << a << " ";
    std::cout << "\nCadena concatenada: ";
    for (auto& a : normalizados) std::cout << a;
    std::cout << "\nSemilla del equipo = " << seed << "\n\n";

    // Longitudes 4,4,5,5,6 alternando A1,A2,A1,A2,A1.
    std::vector<int> lengths = {4, 4, 5, 5, 6};
    std::vector<std::string> alph_names = {"a1", "a2", "a1", "a2", "a1"};

    fb::LCG lcg(seed);
    std::cout << "n | alfabeto | contraseña | sha256\n";
    std::cout << "--------------------------------------------------------------\n";
    for (size_t i = 0; i < lengths.size(); ++i) {
        const std::string& alphabet = fb::alphabet_by_name(alph_names[i]);
        std::string pwd = fb::generate_password(lcg, alphabet, lengths[i]);
        std::string h = fb::sha256_hex(pwd);
        std::cout << lengths[i] << " | " << alph_names[i] << " | " << pwd << " | " << h << "\n";
    }
    std::cout << "\n[RECORDATORIO] Verifique el orden A1/A2 alternado contra el "
                 "enunciado de InteractivaVirtual antes de usar estas instancias.\n";
    return 0;
}

} // namespace

int main(int argc, char** argv) {
    if (argc < 2) { print_usage(); return 1; }
    std::string cmd = argv[1];

    if (cmd == "hash") return cmd_hash(argc, argv);
    if (cmd == "verify-ref") return cmd_verify_ref();
    if (cmd == "search") return cmd_search(argc, argv);
    if (cmd == "dict") return cmd_dict(argc, argv);
    if (cmd == "gen-instances") return cmd_gen_instances(argc, argv);

    print_usage();
    return 1;
}
