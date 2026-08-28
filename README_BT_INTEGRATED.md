# Módulo FB + BT — Práctica 1

Práctica 1 de Análisis y Diseño de Algoritmos. El repositorio integra los
módulos de Fuerza Bruta (FB), ataque por diccionario y Backtracking (BT).

## Guía rápida (Mac/Linux)

Clonar el repo y entrar:

```bash
git clone https://github.com/jjacevedo/ADA_P1_Acevedo_Contreras_Pena.git
cd ADA_P1_Acevedo_Contreras_Pena
```

Compilar:

```bash
g++ -std=c++17 -O2 -Wall -o ada_p1_fb src/main.cpp
```

Validar FB:

```bash
./ada_p1_fb verify-ref
```

Comandos FB:

```bash
./ada_p1_fb hash abc12
./ada_p1_fb search --alphabet a1 --length 4 --hash <hex>
./ada_p1_fb dict --hash <hex>
./ada_p1_fb gen-instances --apellidos "acevedo,contreras,pena"
```

## Guía rápida (Windows)

Se puede usar Git Bash, WSL o un entorno con g++/MinGW.

En PowerShell:

```powershell
g++ -std=c++17 -O2 -Wall -o ada_p1_fb.exe src/main.cpp
.\ada_p1_fb.exe verify-ref
```

## Estructura de `src/`

- `fb_alphabet.hpp` — alfabetos A1 y A2.
- `fb_hash.hpp` — SHA-256.
- `fb_bruteforce.hpp` — enumeración exhaustiva FB.
- `fb_dictionary.hpp` — ataque por diccionario.
- `fb_seed.hpp` — semilla y generación de instancias.
- `bt_backtracking.hpp` — algoritmo de Backtracking con y sin poda.
- `main.cpp` — CLI de FB y BT.
- `third_party/picosha2.h` — SHA-256.

## Instancias FB del equipo

Semilla = 2124.

Las instancias FB existentes en el repositorio se mantienen sin cambios.

## Módulo BT — Backtracking

### Política del equipo

La semilla del equipo es 2124.

Los parámetros derivados son:

```text
minLower = 2 + (2124 mod 3) = 2
minUpper = 1 + (2124 mod 2) = 1
minDigit = 1 + (2124 mod 3) = 1
minSymbol = 1
```

Además, no se permiten dos caracteres idénticos consecutivos.

### Alfabeto

El enunciado enumera:

```text
a-z
A-Z
0-9
! @ # $ %
```

La implementación usa exactamente esos caracteres.

Nota: el documento dice "69 símbolos", pero los conjuntos enumerados suman
67. No se inventan caracteres adicionales que no estén especificados.

### Funcionamiento

El algoritmo construye la contraseña incrementalmente, agregando un
carácter en cada nivel.

Para cada prefijo:

1. Se comprueba la prohibición de caracteres consecutivos iguales.
2. Se comprueba si el prefijo todavía puede satisfacer los mínimos de la
   política.
3. Si no puede, se poda la rama.
4. Si puede continuar, se llama recursivamente.
5. Al regresar se deshace el último carácter.

La función de factibilidad es la parte principal de la poda.

### Comandos

Política completa, `n=8`:

```bash
./ada_p1_fb bt --length 8 --policy team
```

Instancia de referencia, `n=6`:

```bash
./ada_p1_fb bt --length 6 --policy team
```

Política completa, `n=10`:

```bash
./ada_p1_fb bt --length 10 --policy team
```

Política relajada, solamente `minLower=1`:

```bash
./ada_p1_fb bt --length 8 --policy relaxed
```

La prohibición de caracteres consecutivos se mantiene porque el enunciado
indica que aplica a todas las instancias.

Sin restricciones de composición:

```bash
./ada_p1_fb bt --length 6 --policy none
```

La prohibición de caracteres consecutivos se mantiene también en esta
variante.

### Version sin poda

Para ejecutar físicamente la versión sin poda:

```bash
./ada_p1_fb bt --length 3 --policy relaxed --pruning off
```

Se recomienda usar instancias pequeñas para la versión sin poda.

### Comparación

Para una instancia pequeña, ejecutar ambas versiones:

```bash
./ada_p1_fb bt-compare --length 3 --policy relaxed --run-no-pruning 1
```

Para instancias grandes:

```bash
./ada_p1_fb bt-compare --length 8 --policy team
```

En longitudes grandes se reporta el tamaño teórico del árbol completo y se
ejecuta BT con poda. No se ejecuta automáticamente la versión sin poda para
evitar una exploración exponencial accidental.

## Métricas BT

El programa reporta:

- nodos visitados con poda;
- nodos podados;
- soluciones encontradas;
- tiempo de ejecución;
- nodos teóricos del árbol completo;
- reducción porcentual del espacio;
- y, en instancias pequeñas, nodos y soluciones de la versión sin poda.

## Cinco configuraciones exigidas

El enunciado solicita como mínimo:

| Caso | Política | n |
|---|---|---:|
| 1 | Completa del equipo | 8 |
| 2 | Completa del equipo | 6 |
| 3 | Completa del equipo | 10 |
| 4 | Relajada: solo `minLower=1` | 8 |
| 5 | Sin restricciones de composición | 6 |

Las mediciones reales se deben ejecutar y registrar; el repositorio no
contiene valores inventados.

## Resultados

Archivos:

```text
results/bt_resultados.csv
results/bt_comparacion.csv
```

Script de experimentos pequeños:

```bash
./run_bt_experiments.sh
```

Gráfica:

```bash
python3 plot_bt.py
```

La gráfica se genera después de tener mediciones en
`results/bt_comparacion.csv`.

## Tests

Ejecutar:

```bash
python3 tests/test_bt.py
```

Los tests utilizan instancias pequeñas para poder comparar realmente
Backtracking con poda contra la versión sin poda.

## Complejidad

El árbol completo tiene:

```text
1 + |Sigma| + |Sigma|^2 + ... + |Sigma|^n
```

nodos y `|Sigma|^n` hojas.

El peor caso de Backtracking conserva la cota exponencial:

```text
O(|Sigma|^n)
```

La poda reduce el número de nodos visitados en las instancias donde las
restricciones son útiles, pero no cambia la cota del peor caso.

La memoria auxiliar es:

```text
O(n)
```

por la profundidad de la recursión y el prefijo actual, sin contar
almacenamiento explícito de soluciones.

## Nota

Las instancias y parámetros de BT deben verificarse antes de usarlos en el
informe. El enunciado exige al menos cinco configuraciones, mediciones reales,
una gráfica por módulo y comparación cuantitativa entre poda y no poda.
