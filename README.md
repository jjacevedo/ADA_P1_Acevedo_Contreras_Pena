# ADA_P1_Acevedo_Contreras_Pena

Práctica 1 de **Análisis y Diseño de Algoritmos**.

El proyecto implementa dos estrategias para la búsqueda de contraseñas:

- **FB — Fuerza Bruta:** recorre sistemáticamente el espacio de contraseñas y compara el SHA-256 de cada candidato con el hash objetivo.
- **BT — Backtracking:** construye las contraseñas de forma incremental y utiliza restricciones de política para podar ramas que no pueden producir soluciones válidas.
- **Diccionario:** FB incluye un ataque por diccionario sobre `resources/diccionario.txt`.

---

## 1. Requisitos

Para compilar y ejecutar el proyecto se necesita:

- Compilador compatible con **C++17**, por ejemplo `g++`.
- **Python 3** para ejecutar los tests automatizados.
- `resources/diccionario.txt` para las pruebas de diccionario.

En Windows se recomienda utilizar **PowerShell**.

---

## 2. Compilación

Desde la raíz del repositorio:

```powershell
g++ -std=c++17 -O2 -Wall -o ada_p1_fb src/main.cpp
```

En Windows se genera:

```text
ada_p1_fb.exe
```

En Linux/macOS/WSL se genera:

```text
ada_p1_fb
```

---

## 3. Ejecución del programa

La aplicación se ejecuta mediante:

```text
ada_p1_fb <comando> [opciones]
```

Comandos principales:

```text
hash
verify-ref
search
dict
gen-instances
bt
bt-compare
```

---

# 4. Módulo FB — Fuerza Bruta

## 4.1 Calcular SHA-256

Para obtener el SHA-256 de una contraseña:

```powershell
.\ada_p1_fb.exe hash abc12
```

---

## 4.2 Instancia de referencia

El proyecto incluye una instancia de referencia:

```text
Contraseña: abc12
Alfabeto: A2
Longitud: 5
Hash:
8d51feb34e3e69f6fa6dffc577e2c60490cf9a7fcd835f9f6af1505b71d74773
```

Ejecutar:

```powershell
.\ada_p1_fb.exe verify-ref
```

La ejecución debe indicar que la contraseña fue encontrada.

---

## 4.3 Búsqueda por Fuerza Bruta

Sintaxis:

```powershell
.\ada_p1_fb.exe search --alphabet <a1|a2> --length <N> --hash <SHA256>
```

Ejemplo:

```powershell
.\ada_p1_fb.exe search --alphabet a1 --length 2 --hash <SHA256>
```

El programa reporta información como:

```text
alfabeto
sigma
n
encontrada
password
candidatos_evaluados
tiempo_ms
```

Los alfabetos utilizados son:

```text
A1 = 26 caracteres
A2 = 36 caracteres
```

Por tanto, el espacio de búsqueda es:

```text
A1: 26^n
A2: 36^n
```

La complejidad teórica de la búsqueda es:

```text
T(n) = O(|Σ|^n)
```

---

## 4.4 Ataque por diccionario

El ataque por diccionario utiliza:

```text
resources/diccionario.txt
```

Sintaxis:

```powershell
.\ada_p1_fb.exe dict --hash <SHA256>
```

A diferencia de Fuerza Bruta, el diccionario no es exhaustivo: solamente puede encontrar una contraseña si esta se encuentra dentro de `diccionario.txt`.

Durante la experimentación se utilizó el archivo de diccionario disponible en el repositorio y se empleó `access123` como caso de control, ya que se encuentra dentro de la lista.

---

## 4.5 Generación de instancias deterministas

Las instancias del equipo se generan mediante:

```powershell
.\ada_p1_fb.exe gen-instances --apellidos "acevedo,contreras,pena"
```

La semilla utilizada es:

```text
2124
```

Las instancias generadas son:

| n | Alfabeto | Contraseña | SHA-256 |
|---:|:---:|:---|:---|
| 4 | A1 | `slaz` | `d477fe9fdf5725d7ff091744b9662732735b32165ba8978fa116f176b5a25dfc` |
| 4 | A2 | `y70p` | `dc319045ebdcd8bd3af2e6459de2975233a05e371cd5e7ec3f8b88a5a43473a0` |
| 5 | A1 | `ixuxs` | `c16a61c136f28aa6f95c87747ab2f4a13ab7106babf6f62db7873200960c8bae` |
| 5 | A2 | `bgdaf` | `d6809ac7b6f13c1d90add5ef7fb3f2b8599b112809c50ed19251d9d68c14517f` |
| 6 | A1 | `ixmbgr` | `70bebf0bcde96437c8891522b46033087e12f7bfad3421cd1f068b3ee820b8c1` |

Estas instancias permiten reproducir las pruebas experimentales.

---

# 5. Módulo BT — Backtracking

BT construye las contraseñas carácter por carácter utilizando un alfabeto de 67 caracteres:

```text
26 minúsculas
26 mayúsculas
10 dígitos
5 símbolos: ! @ # $ %
```

Por tanto:

```text
|Σ| = 67
```

La implementación se encuentra en:

```text
src/bt_backtracking.hpp
```

Además, se mantiene la restricción de no permitir caracteres consecutivos iguales.

---

## 5.1 Política `team`

La política del equipo utiliza:

```text
minLower = 2
minUpper = 1
minDigit = 1
minSymbol = 1
noConsecutive = 1
```

La longitud mínima posible es:

```text
2 + 1 + 1 + 1 = 5
```

Ejecutar:

```powershell
.\ada_p1_fb.exe bt --length 5 --policy team
```

---

## 5.2 Política `relaxed`

La política utiliza:

```text
minLower = 1
minUpper = 0
minDigit = 0
minSymbol = 0
noConsecutive = 1
```

Ejecutar:

```powershell
.\ada_p1_fb.exe bt --length 3 --policy relaxed
```

---

## 5.3 Política `none`

La política utiliza:

```text
minLower = 0
minUpper = 0
minDigit = 0
minSymbol = 0
noConsecutive = 1
```

Aunque se denomina `none`, la restricción `noConsecutive` permanece activa.

Ejecutar:

```powershell
.\ada_p1_fb.exe bt --length 3 --policy none
```

---

## 5.4 Activar o desactivar la poda

Con poda:

```powershell
.\ada_p1_fb.exe bt --length 3 --policy relaxed --pruning on
```

Sin poda:

```powershell
.\ada_p1_fb.exe bt --length 3 --policy relaxed --pruning off
```

La versión sin poda realiza una exploración exhaustiva del árbol.

La versión con poda descarta ramas cuando el prefijo actual ya no puede convertirse en una solución válida.

---

# 6. Comparación BT con y sin poda

Para comparar ambas estrategias:

```powershell
.\ada_p1_fb.exe bt-compare --length <N> --policy <team|relaxed|none> --run-no-pruning 1
```

Durante la experimentación se utilizaron:

```powershell
.\ada_p1_fb.exe bt-compare --length 3 --policy none --run-no-pruning 1
```

```powershell
.\ada_p1_fb.exe bt-compare --length 3 --policy relaxed --run-no-pruning 1
```

```powershell
.\ada_p1_fb.exe bt-compare --length 3 --policy team --run-no-pruning 1
```

```powershell
.\ada_p1_fb.exe bt-compare --length 4 --policy relaxed --run-no-pruning 1
```

```powershell
.\ada_p1_fb.exe bt-compare --length 5 --policy team --run-no-pruning 1
```

El comando permite comparar:

- nodos del árbol completo;
- nodos visitados con poda;
- nodos podados;
- reducción porcentual;
- soluciones encontradas;
- tiempo con poda;
- tiempo sin poda.

La cantidad de soluciones debe coincidir entre ambas versiones.

---

# 7. Tests automatizados

Los tests se encuentran en:

```text
tests/
├── test_fb.py
└── test_bt.py
```

Los scripts compilan el programa y ejecutan diferentes comandos de la CLI para validar su comportamiento.

## 7.1 Tests de FB

Ejecutar:

```powershell
python tests/test_fb.py
```

El test verifica, entre otros:

- cálculo de SHA-256;
- instancia de referencia;
- búsqueda directa;
- casos de búsqueda;
- generación determinista de instancias.

Resultado esperado:

```text
[OK] Tests automatizados de FB ejecutados correctamente.
```

## 7.2 Tests de BT

Ejecutar:

```powershell
python tests/test_bt.py
```

El test verifica:

- política `none`;
- restricción `noConsecutive`;
- conteo de soluciones;
- igualdad de resultados con y sin poda;
- reducción del espacio de búsqueda;
- parámetros de `team`;
- comportamiento de `team` con longitud 5.

No se recomienda utilizar tamaños grandes en los tests automatizados debido al costo computacional.

---

# 8. Resultados de experimentación

## 8.1 Fuerza Bruta

La experimentación confirmó el crecimiento exponencial:

```text
T(n) = O(|Σ|^n)
```

Resultados representativos:

| Alfabeto | n | Candidatos | Tiempo aproximado |
|:---:|---:|---:|---:|
| A1 | 3 | 17,576 | 38 ms |
| A1 | 4 | 456,976 | 557 ms |
| A1 | 5 | 11,881,376 | ~14.8 s |
| A1 | 6 | 308,915,776 | ~11.4 min |
| A2 | 3 | 46,656 | 57 ms |
| A2 | 4 | 1,679,616 | ~2.1 s |
| A2 | 5 | 60,466,176 | ~75 s |

El crecimiento teórico al aumentar una posición es aproximadamente:

```text
A1 → ×26
A2 → ×36
```

Los tiempos experimentales pueden presentar pequeñas variaciones debido a las condiciones de ejecución del sistema.

---

## 8.2 Backtracking

Resultados representativos obtenidos durante las pruebas:

| Política | n | Nodos visitados | Nodos podados | Soluciones | Tiempo |
|:---|---:|---:|---:|---:|---:|
| team | 3 | 1 | 1 | 0 | 0.0001 ms |
| team | 4 | 1 | 1 | 0 | 0.0001 ms |
| relaxed | 3 | 230,742 | 70,089 | 226,252 | ~5 ms |
| team | 5 | 56,389,826 | 243,302,917 | 51,916,800 | ~2.45–2.52 s |
| team | 6 | 9,454,738,424 | 28,794,330,385 | 8,883,856,800 | ~345.76 s |
| none | 6 | 85,197,148,478 | 1,290,865,885 | 83,906,282,592 | ~23.13 min |

Los resultados muestran que el costo también crece rápidamente con `n`, pero las políticas restrictivas pueden reducir considerablemente el espacio explorado.

---

## 8.3 Comparación BT con y sin poda

Uno de los resultados más importantes fue:

### `team`, n=5

```text
Árbol completo:       1,370,581,548
Nodos con poda:          56,389,826
Nodos podados:          243,302,917
Soluciones:               51,916,800
```

Reducción del espacio de búsqueda:

```text
95.8857 %
```

Tiempos:

```text
Con poda:    ~2.55 s
Sin poda:   ~37.00 s
```

La ejecución con poda fue aproximadamente:

```text
14.5 veces más rápida
```

Además:

```text
soluciones_con_poda = soluciones_sin_poda
```

lo que confirma que la poda no eliminó soluciones válidas en esta instancia.

---

## 8.4 Punto de costo computacional

Durante la experimentación se identificó `n=6` como un punto en el que el costo empieza a ser poco manejable para una campaña normal de pruebas.

Ejemplos:

```text
BT team, n=5 → ~2.45 s
BT team, n=6 → ~345.76 s ≈ 5.76 min

BT none, n=6 → ~23.13 min
```

Por esta razón, no se recomienda repetir innecesariamente pruebas exhaustivas de tamaños superiores.

También se intentó trabajar con tamaños mayores, pero el costo computacional dejó de ser práctico para el equipo utilizado.

---

# 9. Gráficas

Las gráficas utilizadas en la experimentación se encuentran en:

```text
results/
```

## 9.1 FB — Tiempo vs. tamaño

Archivo:

```text
results/tiempo_vs_n_fb.png
```

## 9.2 FB vs. diccionario

Archivo:

```text
results/fb_vs_diccionario.png
```

## 9.3 BT — Tiempo vs. longitud

Script:

```text
results/graficar_bt.py
```

Ejecutar:

```powershell
python results/graficar_bt.py
```

Genera:

```text
results/bt_tiempo_vs_n.png
```

La gráfica utiliza escala logarítmica en el eje Y debido a la gran diferencia entre los tiempos de las distintas instancias.

## 9.4 BT — Poda vs. sin poda

Script:

```text
results/graficar_bt_comparacion.py
```

Ejecutar:

```powershell
python results/graficar_bt_comparacion.py
```

Genera:

```text
results/bt_poda_vs_sin_poda.png
```

---

# 10. Documentación QA

El reporte detallado de QA se encuentra en:

```text
report/QA_FB_BT.md
```

Este documento contiene:

- casos funcionales de FB;
- casos de robustez;
- pruebas de entrada inválida;
- pruebas de BT;
- validaciones matemáticas;
- comparación con y sin poda;
- comparación FB/diccionario;
- análisis de rendimiento;
- análisis del crecimiento;
- punto de costo computacional no manejable;
- matriz consolidada de resultados.

---

# 11. Estructura del proyecto

```text
ADA_P1_Acevedo_Contreras_Pena/
│
├── README.md
│
├── src/
│   ├── main.cpp
│   ├── fb_alphabet.hpp
│   ├── fb_hash.hpp
│   ├── fb_bruteforce.hpp
│   ├── fb_dictionary.hpp
│   ├── fb_seed.hpp
│   ├── bt_backtracking.hpp
│   └── third_party/
│
├── tests/
│   ├── test_fb.py
│   └── test_bt.py
│
├── resources/
│   └── diccionario.txt
│
├── results/
│   ├── tiempo_vs_n_fb.png
│   ├── fb_vs_diccionario.png
│   ├── bt_tiempo_vs_n.png
│   ├── bt_poda_vs_sin_poda.png
│   ├── graficar_bt.py
│   └── graficar_bt_comparacion.py
│
└── report/
    └── QA_FB_BT.md
```

---

# 12. Ejecución rápida

Después de clonar el repositorio:

### Compilar

```powershell
g++ -std=c++17 -O2 -Wall -o ada_p1_fb src/main.cpp
```

### Verificar FB

```powershell
.\ada_p1_fb.exe verify-ref
```

### Ejecutar tests de FB

```powershell
python tests/test_fb.py
```

### Ejecutar tests de BT

```powershell
python tests/test_bt.py
```

### Ejecutar una prueba BT

```powershell
.\ada_p1_fb.exe bt --length 5 --policy team
```

### Comparar BT con y sin poda

```powershell
.\ada_p1_fb.exe bt-compare --length 5 --policy team --run-no-pruning 1
```

### Generar gráficas de BT

```powershell
python results/graficar_bt.py
```

```powershell
python results/graficar_bt_comparacion.py
```

---

# 13. Consideraciones

Los experimentos de tamaños grandes pueden requerir varios minutos.

Esto es consecuencia del crecimiento exponencial del espacio de búsqueda.

Durante la experimentación se identificó `n=6` como un punto en el que el costo comienza a ser poco manejable para una campaña normal de pruebas en el equipo utilizado.

Por esta razón, no se recomienda repetir innecesariamente las pruebas exhaustivas de tamaños superiores.

Los tiempos reportados corresponden a ejecuciones realizadas en el equipo de desarrollo y pueden variar ligeramente entre ejecuciones.

Para consultar el detalle completo de los casos de prueba y sus resultados, revisar:

```text
report/QA_FB_BT.md
```
