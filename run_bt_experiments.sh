# Experimentos basicos del modulo BT.
# No ejecuta sin poda para n grandes.
set -e
cd "$(dirname "$0")"

BIN=./ada_p1_fb
CSV=results/bt_resultados.csv
CMP=results/bt_comparacion.csv

mkdir -p results

echo "caso,n,politica,poda,nodos_visitados,nodos_podados,soluciones,tiempo_ms" > "$CSV"
echo "caso,n,politica,nodos_arbol_completo,nodos_con_poda,nodos_podados,reduccion_porcentaje,soluciones_con_poda,tiempo_con_poda_ms" > "$CMP"

run_bt() {
    local caso=$1
    local n=$2
    local policy=$3

    local out
    out=$($BIN bt --length "$n" --policy "$policy" --pruning on)

    local nodes=$(echo "$out" | grep '^nodos_visitados=' | cut -d= -f2)
    local pruned=$(echo "$out" | grep '^nodos_podados=' | cut -d= -f2)
    local solutions=$(echo "$out" | grep '^soluciones=' | cut -d= -f2)
    local time=$(echo "$out" | grep '^tiempo_ms=' | cut -d= -f2)

    echo "$caso,$n,$policy,1,$nodes,$pruned,$solutions,$time" >> "$CSV"
}

run_compare() {
    local caso=$1
    local n=$2
    local policy=$3

    local out
    out=$($BIN bt-compare --length "$n" --policy "$policy" --run-no-pruning 1)

    local tree=$(echo "$out" | grep '^nodos_arbol_completo=' | cut -d= -f2)
    local nodes=$(echo "$out" | grep '^nodos_con_poda=' | cut -d= -f2)
    local pruned=$(echo "$out" | grep '^nodos_podados=' | cut -d= -f2)
    local reduction=$(echo "$out" | grep '^reduccion_nodos_porcentaje=' | cut -d= -f2)
    local solutions=$(echo "$out" | grep '^soluciones_con_poda=' | cut -d= -f2)
    local time=$(echo "$out" | grep '^tiempo_con_poda_ms=' | cut -d= -f2)

    echo "$caso,$n,$policy,$tree,$nodes,$pruned,$reduction,$solutions,$time" >> "$CMP"
}

# Casos pequeños reproducibles.
run_bt "sin_restricciones_n3" 3 none
run_bt "relajada_n3" 3 relaxed
run_compare "comparacion_relaxed_n3" 3 relaxed

echo "Resultados escritos en:"
echo "  $CSV"
echo "  $CMP"
echo
echo "Para n=6,8,10 use bt con cuidado: el espacio crece exponencialmente."
