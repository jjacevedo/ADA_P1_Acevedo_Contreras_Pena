# Módulo FB — Fuerza Bruta

Avance del Módulo FB de la Práctica 1 (Análisis y Diseño de Algoritmos).
Este README cubre solo este módulo; se combina con el del compañero de BT
cuando se junten los dos módulos en el repositorio final.

## Guía rápida (Mac/Linux)

1. Clonar el repo y entrar a la carpeta:
   ```bash
   git clone https://github.com/jjacevedo/ADA_P1_Acevedo_Contreras_Pena.git
   cd ADA_P1_Acevedo_Contreras_Pena
   ```
2. Compilar:
   ```bash
   g++ -std=c++17 -O2 -Wall -o ada_p1_fb src/main.cpp
   ```
3. Validar que compiló bien:
   ```bash
   ./ada_p1_fb verify-ref
   ```
4. Ejecutar cualquier comando:
   ```bash
   ./ada_p1_fb hash abc12                                  # calcula un hash SHA-256
   ./ada_p1_fb search --alphabet a1 --length 4 --hash <hex> # fuerza bruta pura
   ./ada_p1_fb dict --hash <hex>                            # ataque por diccionario
   ./ada_p1_fb gen-instances --apellidos "acevedo,contreras,peña"  # instancias del equipo
   ```
5. Reproducir los experimentos:
   ```bash
   ./run_experiments.sh
   ```

## Guía rápida (Windows)

Igual que arriba, pero antes hace falta un entorno con `g++` y bash. Lo más simple:

1. Instalar Git Bash (viene con [Git for Windows](https://git-scm.com/download/win)) o usar WSL.
2. Instalar un compilador C++17: MSYS2/MinGW-w64, o el `g++` que trae WSL (Ubuntu: `sudo apt install g++`).
3. Abrir Git Bash (o la terminal de WSL) y seguir los mismos pasos de la guía de Mac/Linux — los comandos son idénticos ahí adentro.

Sin Git Bash/WSL (cmd o PowerShell nativos):

- El binario se llama `ada_p1_fb.exe`, no `ada_p1_fb`.
- En PowerShell se invoca `.\ada_p1_fb.exe`, no `./ada_p1_fb`.
- `run_experiments.sh` no corre (es bash) — hay que correr los comandos de `./ada_p1_fb ...` a mano, o usar Git Bash/WSL para ese script.

## Estructura (dentro de `src/`)

- `fb_alphabet.hpp` — alfabetos A1 y A2.
- `fb_hash.hpp` — cálculo del SHA-256.
- `fb_bruteforce.hpp` — algoritmo de enumeración exhaustiva.
- `fb_dictionary.hpp` — ataque por diccionario.
- `fb_seed.hpp` — semilla del equipo + generador de instancias.
- `main.cpp` — CLI que expone los comandos de arriba.
- `third_party/picosha2.h` — librería de hash SHA-256.

## Instancias del equipo (Acevedo, Contreras, Peña)

Semilla = 2124. Generadas con `gen-instances`, orden A1, A2, A1, A2, A1:

| n | alfabeto | contraseña | hash |
|---|----------|-----------|------|
| 4 | A1 | slaz | d477fe9fdf5725d7ff091744b9662732735b32165ba8978fa116f176b5a25dfc |
| 4 | A2 | y70p | dc319045ebdcd8bd3af2e6459de2975233a05e371cd5e7ec3f8b88a5a43473a0 |
| 5 | A1 | ixuxs | c16a61c136f28aa6f95c87747ab2f4a13ab7106babf6f62db7873200960c8bae |
| 5 | A2 | bgdaf | d6809ac7b6f13c1d90add5ef7fb3f2b8599b112809c50ed19251d9d68c14517f |
| 6 | A1 | ixmbgr | 70bebf0bcde96437c8891522b46033087e12f7bfad3421cd1f068b3ee820b8c1 |

## Experimentación

`run_experiments.sh` corre el peor caso (6 de las 7 configuraciones — a1
n=6 se corre aparte, ver más abajo) y las instancias reales para A1/A2
n=3,4,5, más la comparación contra el diccionario, y escribe:

- `results/tiempos_fb.csv`
- `results/comparacion_fb_diccionario.csv`
- `results/instancias_txt/` — un .txt por corrida con la misma salida que imprime el CLI

Las gráficas se regeneran con:

```bash
python3 plot_tiempos_fb.py
python3 plot_fb_vs_diccionario.py
```

A1 n=6 no está en el script (tarda ~11.4 min) — se corrió aparte y sus filas
ya están agregadas a mano en los CSV. Comandos para reproducirlo:

```bash
./ada_p1_fb search --alphabet a1 --length 6 --hash 0000000000000000000000000000000000000000000000000000000000000000  # peor caso, ~11.4 min
./ada_p1_fb search --alphabet a1 --length 6 --hash 70bebf0bcde96437c8891522b46033087e12f7bfad3421cd1f068b3ee820b8c1  # instancia real, ~4 min
```

### Resultados (peor caso)

| Alfabeto | n | Candidatos | Tiempo |
|---|---|---|---|
| A1 | 3 | 17,576 | 38.2 ms |
| A1 | 4 | 456,976 | 556.8 ms |
| A1 | 5 | 11,881,376 | ~14.8 s |
| A1 | 6 | 308,915,776 | **683.4 s (~11.4 min)** |
| A2 | 3 | 46,656 | 56.8 ms |
| A2 | 4 | 1,679,616 | ~2.1 s |
| A2 | 5 | 60,466,176 | ~75.0 s |

Candidatos evaluados = exactamente |Σ|ⁿ en los 7 casos. Factor de
crecimiento medido al subir n en 1: A1 entre ×14.6 y ×46.3 según el rango
(teórico ×26), A2 ≈ ×36 (teórico ×36).

### Resultados (fuerza bruta vs. diccionario)

Ninguna de las 5 instancias del equipo está en `diccionario.txt` (0/5). La
fuerza bruta sí las encuentra las 5, tardando entre 393.7 ms y 243.2 s. El
diccionario responde en menos de 1.5 ms siempre. Caso de control
(`acceso123`, que sí está en la lista): se encuentra en 0.016 ms.

## Pendiente

- Redactar el informe (secciones 3–15 según la estructura del PDF).
- Tests automatizados en `tests/` (a cargo del compañero encargado de pruebas).



## Backtracking (BT)

### Descripción

El módulo de Backtracking construye las contraseñas de forma incremental,
explorando un árbol de bssqueda y descartando ramas que no pueden producir
una solución valida.

La implementación se encuentra en:

`src/bt_backtracking.hpp`

### Política del equipo

La política utilizada por el equipo es:

- `minLower = 2`
- `minUpper = 1`
- `minDigit = 1`
- `minSymbol = 1`
- No se permiten caracteres consecutivos iguales.

La semilla utilizada es `2124`.

### Compilación

```bash
g++ -std=c++17 -O2 -Wall -o ada_p1_fb src/main.cpp
