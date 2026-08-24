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

echo "alfabeto,n,tipo_caso,candidatos_evaluados,tiempo_ms,encontrada" > "$CSV"

run_worst_case() {
    local alf=$1
    local n=$2
    echo ">> peor caso: $alf n=$n"
    local out
    out=$($BIN search --alphabet "$alf" --length "$n" --hash "$NOHASH")
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
# Corre el diccionario contra las mismas 5 instancias del equipo (n=6 aparte).
# Ninguna debería estar en diccionario.txt (se generan por LCG, no son
# palabras "comunes"), así que se agrega un caso de control que SÍ está en la
# lista, para mostrar el contraste completo (éxito vs. no éxito).
DICT_CSV=results/comparacion_fb_diccionario.csv
echo "alfabeto,n,candidatos_dict,tiempo_dict_ms,encontrada_dict" > "$DICT_CSV"

run_dict() {
    local alf=$1
    local n=$2
    local hash=$3
    echo ">> diccionario: $alf n=$n"
    local out
    out=$($BIN dict --hash "$hash")
    local cand=$(echo "$out" | grep candidatos_evaluados | cut -d= -f2)
    local t=$(echo "$out" | grep tiempo_ms | cut -d= -f2)
    local found=$(echo "$out" | grep encontrada | cut -d= -f2)
    echo "$alf,$n,$cand,$t,$found" >> "$DICT_CSV"
}

run_dict a1 4 d477fe9fdf5725d7ff091744b9662732735b32165ba8978fa116f176b5a25dfc
run_dict a2 4 dc319045ebdcd8bd3af2e6459de2975233a05e371cd5e7ec3f8b88a5a43473a0
run_dict a1 5 c16a61c136f28aa6f95c87747ab2f4a13ab7106babf6f62db7873200960c8bae
run_dict a2 5 d6809ac7b6f13c1d90add5ef7fb3f2b8599b112809c50ed19251d9d68c14517f
# a1 n=6 aparte

# Caso de control: contraseña que SÍ está en resources/diccionario.txt.
CONTROL_HASH=$($BIN hash acceso123)
run_dict control 9 "$CONTROL_HASH"

echo "Listo. Comparación en $DICT_CSV"
