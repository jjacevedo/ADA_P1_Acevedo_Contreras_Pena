#!/bin/bash
# run_experiments.sh
# Corre las configuraciones de fuerza bruta y escribe results/tiempos_fb.csv,
# una fila por corrida.
#
# Dos tipos de fila:
#   - peor_caso: hash inexistente (todo ceros), agota Σ^n. Sirve para
#     contrastar el crecimiento medido contra la cota teórica |Σ|^n.
#   - instancia_equipo: una de las 5 contraseñas reales del equipo,
#     calculadas a partir de la semilla. Termina apenas la encuentra.
#
# Uso: ./run_experiments.sh
set -e
cd "$(dirname "$0")"

BIN=./ada_p1_fb
CSV=results/tiempos_fb.csv
NOHASH="0000000000000000000000000000000000000000000000000000000000000000"
TXT_DIR=results/instancias_txt
mkdir -p "$TXT_DIR"

echo "alfabeto,n,tipo_caso,candidatos_evaluados,tiempo_ms,encontrada" > "$CSV"

run_worst_case() {
    local alf=$1
    local n=$2
    echo ">> peor caso: $alf n=$n"
    local out
    out=$($BIN search --alphabet "$alf" --length "$n" --hash "$NOHASH")
    echo "$out" > "$TXT_DIR/${alf}_n${n}_peor_caso.txt"
    local cand=$(echo "$out" | grep candidatos_evaluados | cut -d= -f2)
    local t=$(echo "$out" | grep tiempo_ms | cut -d= -f2)
    local found=$(echo "$out" | grep encontrada | cut -d= -f2)
    echo "$alf,$n,peor_caso,$cand,$t,$found" >> "$CSV"
}

run_team_instance() {
    local alf=$1
    local n=$2
    local hash=$3
    echo ">> instancia del equipo: $alf n=$n"
    local out
    out=$($BIN search --alphabet "$alf" --length "$n" --hash "$hash")
    echo "$out" > "$TXT_DIR/${alf}_n${n}_instancia_equipo.txt"
    local cand=$(echo "$out" | grep candidatos_evaluados | cut -d= -f2)
    local t=$(echo "$out" | grep tiempo_ms | cut -d= -f2)
    local found=$(echo "$out" | grep encontrada | cut -d= -f2)
    echo "$alf,$n,instancia_equipo,$cand,$t,$found" >> "$CSV"
}

# --- Peor caso: 6 de las 7 configuraciones (a1 n=6 se corre aparte, ~11 min) ---
run_worst_case a1 3
run_worst_case a1 4
run_worst_case a1 5
run_worst_case a2 3
run_worst_case a2 4
run_worst_case a2 5

# --- Instancias reales del equipo, semilla=2124 (a1 n=6 aparte, ~4 min) ---
run_team_instance a1 4 d477fe9fdf5725d7ff091744b9662732735b32165ba8978fa116f176b5a25dfc
run_team_instance a2 4 dc319045ebdcd8bd3af2e6459de2975233a05e371cd5e7ec3f8b88a5a43473a0
run_team_instance a1 5 c16a61c136f28aa6f95c87747ab2f4a13ab7106babf6f62db7873200960c8bae
run_team_instance a2 5 d6809ac7b6f13c1d90add5ef7fb3f2b8599b112809c50ed19251d9d68c14517f
# Los comandos exactos para a1 n=6 (peor caso e instancia real) están en el README.

echo "Listo. Resultados en $CSV"

# --- Comparación fuerza bruta vs. ataque por diccionario ---
# Corre FB y diccionario contra el MISMO hash y deja ambos en la misma fila
# (Sección 8.1: mismo conjunto de hashes objetivo para las dos estrategias).
# Ninguna de las 5 instancias del equipo debería estar en diccionario.txt (se
# generan por LCG, no son palabras "comunes"), así que se agrega un caso de
# control que SÍ está en la lista, para mostrar el contraste completo.
DICT_CSV=results/comparacion_fb_diccionario.csv
echo "alfabeto,n,candidatos_fb,tiempo_fb_ms,encontrada_fb,candidatos_dict,tiempo_dict_ms,encontrada_dict" > "$DICT_CSV"

run_comparison() {
    local alf=$1
    local n=$2
    local hash=$3
    echo ">> comparación: $alf n=$n"
    local fb_out fb_cand fb_t fb_found
    fb_out=$($BIN search --alphabet "$alf" --length "$n" --hash "$hash")
    fb_cand=$(echo "$fb_out" | grep candidatos_evaluados | cut -d= -f2)
    fb_t=$(echo "$fb_out" | grep tiempo_ms | cut -d= -f2)
    fb_found=$(echo "$fb_out" | grep encontrada | cut -d= -f2)
    local dict_out dict_cand dict_t dict_found
    dict_out=$($BIN dict --hash "$hash")
    dict_cand=$(echo "$dict_out" | grep candidatos_evaluados | cut -d= -f2)
    dict_t=$(echo "$dict_out" | grep tiempo_ms | cut -d= -f2)
    dict_found=$(echo "$dict_out" | grep encontrada | cut -d= -f2)
    {
        echo "=== fuerza bruta ==="
        echo "$fb_out"
        echo
        echo "=== diccionario ==="
        echo "$dict_out"
    } > "$TXT_DIR/comparacion_${alf}_n${n}.txt"
    echo "$alf,$n,$fb_cand,$fb_t,$fb_found,$dict_cand,$dict_t,$dict_found" >> "$DICT_CSV"
}

run_comparison a1 4 d477fe9fdf5725d7ff091744b9662732735b32165ba8978fa116f176b5a25dfc
run_comparison a2 4 dc319045ebdcd8bd3af2e6459de2975233a05e371cd5e7ec3f8b88a5a43473a0
run_comparison a1 5 c16a61c136f28aa6f95c87747ab2f4a13ab7106babf6f62db7873200960c8bae
run_comparison a2 5 d6809ac7b6f13c1d90add5ef7fb3f2b8599b112809c50ed19251d9d68c14517f
# a1 n=6 aparte (FB tarda ~4 min): fila agregada a mano con los valores de
# tiempos_fb.csv y una corrida de dict suelta, igual que antes.

# Caso de control: contraseña que SÍ está en resources/diccionario.txt.
# No tiene comparación FB real (n=9 sobre A1 es intratable: 26^9), es solo
# para mostrar que el diccionario sí encuentra lo que contiene.
echo ">> comparación: control n=9 (solo diccionario)"
CONTROL_HASH=$($BIN hash acceso123)
control_out=$($BIN dict --hash "$CONTROL_HASH")
control_cand=$(echo "$control_out" | grep candidatos_evaluados | cut -d= -f2)
control_t=$(echo "$control_out" | grep tiempo_ms | cut -d= -f2)
control_found=$(echo "$control_out" | grep encontrada | cut -d= -f2)
echo "=== diccionario (control: acceso123) ===" > "$TXT_DIR/comparacion_control_n9.txt"
echo "$control_out" >> "$TXT_DIR/comparacion_control_n9.txt"
echo "control,9,n/a,n/a,n/a,$control_cand,$control_t,$control_found" >> "$DICT_CSV"

echo "Listo. Comparación en $DICT_CSV"
