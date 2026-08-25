# Módulo FB — Fuerza Bruta

Avance del Módulo FB de la Práctica 1 (Análisis y Diseño de Algoritmos).
Este README cubre solo este módulo; cuando se una con el Módulo BT del
compañero, se combina en un único README de todo el repositorio (Sección 11).

## Compilar

```bash
g++ -std=c++17 -O2 -Wall -o ada_p1_fb src/main.cpp
```

## Ejecutar

```bash
./ada_p1_fb verify-ref                                  # valida contra la instancia de referencia del curso
./ada_p1_fb hash abc12                                  # calcula un hash SHA-256
./ada_p1_fb search --alphabet a1 --length 4 --hash <hex> # fuerza bruta pura
./ada_p1_fb dict --hash <hex>                            # ataque por diccionario
./ada_p1_fb gen-instances --apellidos "acevedo,contreras,peña"  # instancias del equipo (Sección 9.1)
```

## Estructura (dentro de `src/`)

- `fb_alphabet.hpp` — alfabetos A1 y A2.
- `fb_hash.hpp` — cálculo del SHA-256.
- `fb_bruteforce.hpp` — algoritmo de enumeración exhaustiva.
- `fb_dictionary.hpp` — ataque por diccionario.
- `fb_seed.hpp` — semilla del equipo + generador de instancias.
- `main.cpp` — CLI que expone los comandos de arriba.
- `third_party/picosha2.h` — librería de hash SHA-256 (dominio público/MIT).

Las carpetas de nivel superior (`src/`, `tests/`, `resources/`, `results/`,
`report/`) y `src/third_party/` tienen esos nombres exactos porque los exige
la Sección 11 del enunciado (es un criterio evaluado de la rúbrica) — no se
pueden renombrar.

## Instancias del equipo (Acevedo, Contreras, Peña)

Semilla = 2124. Generadas con `gen-instances` (orden A1, A2, A1, A2, A1 — confirmado oficialmente por el profesor en el foro del curso):

| n | alfabeto | contraseña | hash |
|---|----------|-----------|------|
| 4 | A1 | slaz | d477fe9fdf5725d7ff091744b9662732735b32165ba8978fa116f176b5a25dfc |
| 4 | A2 | y70p | dc319045ebdcd8bd3af2e6459de2975233a05e371cd5e7ec3f8b88a5a43473a0 |
| 5 | A1 | ixuxs | c16a61c136f28aa6f95c87747ab2f4a13ab7106babf6f62db7873200960c8bae |
| 5 | A2 | bgdaf | d6809ac7b6f13c1d90add5ef7fb3f2b8599b112809c50ed19251d9d68c14517f |
| 6 | A1 | ixmbgr | 70bebf0bcde96437c8891522b46033087e12f7bfad3421cd1f068b3ee820b8c1 |

## Experimentación (Secciones 8 y 8.1)

`run_experiments.sh` corre automáticamente el peor caso (hash inexistente) y
las instancias reales para A1 n=3,4,5 y A2 n=3,4,5, más la comparación contra
el diccionario, y escribe:

- `results/tiempos_fb.csv` — tiempo, candidatos evaluados, peor caso vs. instancia real.
- `results/comparacion_fb_diccionario.csv` — fuerza bruta vs. diccionario.
- `results/tiempo_vs_n_fb.png` — gráfica tiempo vs. n (peor caso, ambos alfabetos, con la curva teórica |Σ|ⁿ superpuesta).

**A1 con n=6 NO está en el script** porque agotar el espacio (26⁶ ≈ 309
millones de candidatos) tarda ~11.4 minutos — se corrió aparte y sus filas ya
están agregadas a mano en los CSV. Si necesitan reproducirlo:

```bash
./ada_p1_fb search --alphabet a1 --length 6 --hash 0000000000000000000000000000000000000000000000000000000000000000  # peor caso, ~11.4 min
./ada_p1_fb search --alphabet a1 --length 6 --hash 70bebf0bcde96437c8891522b46033087e12f7bfad3421cd1f068b3ee820b8c1  # instancia real, ~4 min
```

### Resultados (peor caso, las 7 configuraciones de la Sección 9.1)

| Alfabeto | n | Candidatos | Tiempo |
|---|---|---|---|
| A1 | 3 | 17,576 | 38–49 ms |
| A1 | 4 | 456,976 | ~975 ms |
| A1 | 5 | 11,881,376 | ~26 s |
| A1 | 6 | 308,915,776 | **683.4 s (~11.4 min)** |
| A2 | 3 | 46,656 | ~100 ms |
| A2 | 4 | 1,679,616 | ~3.7 s |
| A2 | 5 | 60,466,176 | ~133 s |

Factor de crecimiento medido al subir n en 1: A1 ≈ ×26.3 (teórico ×26), A2 ≈
×36.3 (teórico ×36) — coincide casi exactamente con la cota |Σ|ⁿ.

### Resultados (fuerza bruta vs. diccionario, Sección 8.1)

Ninguna de las 5 instancias reales del equipo está en `diccionario.txt` (0/5
éxito) — son generadas por LCG, no palabras "comunes" — pero el diccionario
responde en ~1–2 ms en cada caso, mientras que la fuerza bruta tardó entre
689 ms (n=4) y 243 s (n=6) en encontrar la misma contraseña. Como caso de
control, `acceso123` (que sí está en la lista) se encuentra en 0.05 ms
evaluando solo 7 candidatos. Esto ilustra la discusión que pide el
enunciado: el diccionario es mucho más rápido, pero **no es exhaustivo** —
si la contraseña real no está en la lista, nunca se encuentra, sin importar
que sí exista dentro de Σⁿ.

## Pendiente

- Redactar el informe (secciones 3–15 según la estructura del PDF).
- Tests automatizados en `tests/` (a cargo del compañero encargado de pruebas).
