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
- `results/comparacion_fb_diccionario.csv` — fuerza bruta vs. diccionario, mismo hash por fila.

La gráfica `results/tiempo_vs_n_fb.png` (tiempo vs. n, peor caso, ambos
alfabetos, con la curva teórica |Σ|ⁿ superpuesta) se genera aparte con:

```bash
python3 plot_tiempos_fb.py         # lee results/tiempos_fb.csv, requiere matplotlib
python3 plot_fb_vs_diccionario.py  # lee results/comparacion_fb_diccionario.csv
```

`results/fb_vs_diccionario.png` (barras FB vs. diccionario por instancia) es
opcional — el enunciado no lo exige para la Sección 8.1, solo tabla y
discusión — pero ayuda a la sustentación. `results/informe_experimentacion_resultados.md`
tiene las tablas y el análisis ya redactados, listos para pegar en
`report/Informe.pdf`.

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
| A1 | 3 | 17,576 | 38.2 ms |
| A1 | 4 | 456,976 | 556.8 ms |
| A1 | 5 | 11,881,376 | ~14.8 s |
| A1 | 6 | 308,915,776 | **683.4 s (~11.4 min)** |
| A2 | 3 | 46,656 | 56.8 ms |
| A2 | 4 | 1,679,616 | ~2.1 s |
| A2 | 5 | 60,466,176 | ~75.0 s |

Candidatos evaluados = exactamente |Σ|ⁿ en los 7 casos (26,036 ✓ y 36ⁿ ✓ sin
falla) — confirma que la enumeración no omite ni repite candidatos.

El **tiempo**, en cambio, no crece parejo: A1 pasa de ×14.6 (n=3→4) a ×26.5
(n=4→5) a ×46.3 (n=5→6), contra un teórico de ×26 fijo. A2 sí se mantiene
cerca (×36.8 y ×35.9, teórico ×36). La explicación: n=3,4,5 de ambos
alfabetos se midieron juntos en una corrida corta (segundos); ahí el
crecimiento converge al ×26/×36 teórico a medida que el costo fijo de
arranque del programa deja de pesar frente al de hashear candidatos. La
corrida de A1 n=6 (~11.4 min) es aparte, medida en otra sesión — un proceso
sostenido de esa duración en un portátil es más susceptible a *thermal
throttling* del CPU, lo que explica el salto a ×46.3: no es que el
algoritmo cambie de complejidad, es ruido de medición en una corrida larga
bajo condiciones distintas a las demás.

### Resultados (fuerza bruta vs. diccionario, Sección 8.1)

Ninguna de las 5 instancias reales del equipo está en `diccionario.txt` (0/5
éxito) — son generadas por LCG, no palabras "comunes". Fuerza bruta sí las
encuentra las 5 (100%), pero tardando de 393.7 ms (`slaz`, n=4) a 243.2 s
(`ixmbgr`, n=6), mientras el diccionario completo (500 candidatos) responde
en menos de 1.5 ms siempre — sea que encuentre o no. Como caso de control,
`acceso123` (que sí está en la lista) se encuentra en 0.016 ms evaluando
solo 7 candidatos. Esto ilustra la discusión que pide el enunciado: el
diccionario es órdenes de magnitud más rápido, pero **no es exhaustivo** —
si la contraseña real no está en la lista, nunca se encuentra, sin importar
que sí exista dentro de Σⁿ; la fuerza bruta es más lenta pero garantiza
encontrarla si el hash pertenece al espacio explorado.

## Pendiente

- Redactar el informe (secciones 3–15 según la estructura del PDF).
- Tests automatizados en `tests/` (a cargo del compañero encargado de pruebas).
