# COMO MONTAR ESTE PAQUETE EN TU REPO

Este paquete contiene los archivos de BT listos para copiar sobre tu repositorio
actual.

1. Reemplaza `src/main.cpp` por el incluido aquí.
2. Copia `src/bt_backtracking.hpp` a tu `src/`.
3. Copia `tests/test_bt.py` a tu `tests/`.
4. Copia los dos CSV a `results/`.
5. Copia `run_bt_experiments.sh` y `plot_bt.py` a la raíz.
6. NO reemplaces los otros archivos FB (`fb_hash.hpp`, `fb_alphabet.hpp`,
   `fb_bruteforce.hpp`, `fb_dictionary.hpp`, `fb_seed.hpp`).
7. Para el README, usa `README_BT_INTEGRATED.md` como referencia y agrega
   su sección BT al README que ya tienen si quieres conservar todo el texto
   histórico de FB.

Compilación:

g++ -std=c++17 -O2 -Wall -o ada_p1_fb src/main.cpp

Prueba:

python3 tests/test_bt.py

La versión sin poda solo debe ejecutarse en instancias pequeñas. El script
de experimentos incluido tampoco inventa mediciones.
