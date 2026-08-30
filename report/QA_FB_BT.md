# Documentación QA — Fuerza Bruta y Backtracking

**Proyecto:** `ADA_P1_Acevedo_Contreras_Pena`  
**Fecha de corte:** 30 de agosto de 2026  
**Alcance:** pruebas funcionales, robustez, experimentación y validación de Fuerza Bruta (FB) y Backtracking (BT).

> Este documento consolida resultados ejecutados y observados durante el trabajo. Los cálculos matemáticos marcados como validación independiente son comprobaciones, no ejecuciones experimentales.

---

## 1. Objetivo

Validar el comportamiento funcional de FB y BT, comprobar conteos, evaluar la poda, comparar FB contra diccionario, medir crecimiento temporal y registrar problemas de robustez de la CLI.

---

## 2. Configuración

### Semilla

```text
2124
```

Apellidos:

```text
Acevedo, Contreras, Peña
```

Orden de alfabetos:

```text
A1, A2, A1, A2, A1
```

### Instancias

| n | Alfabeto | Contraseña | SHA-256 |
|---:|:---:|:---|:---|
| 4 | A1 | `slaz` | `d477fe9fdf5725d7ff091744b9662732735b32165ba8978fa116f176b5a25dfc` |
| 4 | A2 | `y70p` | `dc319045ebdcd8bd3af2e6459de2975233a05e371cd5e7ec3f8b88a5a43473a0` |
| 5 | A1 | `ixuxs` | `c16a61c136f28aa6f95c87747ab2f4a13ab7106babf6f62db7873200960c8bae` |
| 5 | A2 | `bgdaf` | `d6809ac7b6f13c1d90add5ef7fb3f2b8599b112809c50ed19251d9d68c14517f` |
| 6 | A1 | `ixmbgr` | `70bebf0bcde96437c8891522b46033087e12f7bfad3421cd1f068b3ee820b8c1` |

### Referencia FB

```text
Contraseña: abc12
Alfabeto: A2
n=5
Hash:
8d51feb34e3e69f6fa6dffc577e2c60490cf9a7fcd835f9f6af1505b71d74773
```

---

# 3. Pruebas funcionales FB

### FB-01 — Referencia

`verify-ref` recupera `abc12` sobre A2, n=5.

Resultado observado:

```text
Encontrada: SI
Contraseña recuperada: abc12
Candidatos evaluados: 50249
Tiempo aproximado: 192.885 ms
```

**PASS.**

### FB-02 — A1, n=2, primera

`aa` ocupa la posición 1.

**PASS.**

### FB-03 — A1, n=2, intermedia

`bb` ocupa la posición 28.

**PASS.**

### FB-04 — A1, n=2, última

`zz` ocupa la posición 676.

```text
26^2 = 676
```

**PASS.**

### FB-05 — A1, n=2, inexistente

El hash inexistente produce `encontrada=0` después de 676 candidatos.

**PASS.**

### FB-06 — A2, n=2

```text
aa -> 1
a0 -> 27
99 -> 1296
```

```text
36^2 = 1296
```

**PASS.**

---

# 4. Ataque por diccionario

El archivo actual utilizado es:

```text
resources/diccionario.txt
```

Como parte de la regresión posterior a los cambios del repositorio se verificó que `access123` se encuentra en el diccionario, en la posición 484.

Hash:

```text
2b5f277cc882b92eb9aedb066234e7feeca309dbb6bd578655ae8029d2328494
```

Comando:

```powershell
.\ada_p1_fb.exe dict --hash 2b5f277cc882b92eb9aedb066234e7feeca309dbb6bd578655ae8029d2328494
```

Resultado:

```text
diccionario=resources/diccionario.txt
encontrada=1
password=access123
candidatos_evaluados=484
tiempo_ms=4.3691
```

**PASS.**

### Conclusión

FB es exhaustiva dentro del espacio definido. El ataque por diccionario solamente prueba las entradas presentes en `diccionario.txt`, por lo que no garantiza encontrar contraseñas que no estén en la lista.

---

# 5. Experimentación FB

| Alfabeto | n | Candidatos | Tiempo aproximado |
|:---:|---:|---:|---:|
| A1 | 3 | 17.576 | 38.2 ms |
| A1 | 4 | 456.976 | 556.8 ms |
| A1 | 5 | 11.881.376 | ~14.8 s |
| A1 | 6 | 308.915.776 | ~11.4 min |
| A2 | 3 | 46.656 | 56.8 ms |
| A2 | 4 | 1.679.616 | ~2.1 s |
| A2 | 5 | 60.466.176 | ~75.0 s |

El número de candidatos coincide con `|Σ|^n` en los peores casos.

A1, n=6 se ejecutó por separado debido a su costo.

---

# 6. Robustez FB

Las pruebas de robustez se evaluaron nuevamente después de integrar el manejo de excepciones de la CLI.

| ID | Entrada | Resultado observado | Estado |
|---|---|---|---|
| CLI-01 | `--alphabet a3` | `[ERROR] Alfabeto desconocido: 'a3' (use 'a1' o 'a2')` | **PASS** |
| CLI-02 | hash vacío | mensaje de argumentos faltantes | **PASS** |
| CLI-03 | hash `abc` | búsqueda ejecutada con 676 candidatos | **FAIL** |
| CLI-04 | hash `xyz` | búsqueda ejecutada con 676 candidatos | **FAIL** |
| CLI-05 | hash de 10 caracteres | búsqueda ejecutada con 676 candidatos | **FAIL** |
| CLI-06 | hash de 64 caracteres con `G` | búsqueda ejecutada con 676 candidatos | **FAIL** |
| CLI-07 | `length=0` | 1 candidato | **FAIL** |
| CLI-08 | `length=-1` | `[ERROR] cannot create std::vector larger than max_size()` | **PASS** |

### Observación sobre CLI-01

El programa maneja actualmente el alfabeto inválido mediante un mensaje de error controlado.

### Observación sobre CLI-08

La entrada negativa produce un error controlado y no termina mediante `abort()`.

---

# 7. Configuración BT

Alfabeto:

```text
26 minúsculas + 26 mayúsculas + 10 dígitos + 5 símbolos = 67
```

Símbolos:

```text
! @ # $ %
```

### `team`

```text
minLower=2
minUpper=1
minDigit=1
minSymbol=1
noConsecutive=1
```

Longitud mínima:

```text
2 + 1 + 1 + 1 = 5
```

### `relaxed`

```text
minLower=1
minUpper=0
minDigit=0
minSymbol=0
noConsecutive=1
```

### `none`

```text
minLower=0
minUpper=0
minDigit=0
minSymbol=0
noConsecutive=1
```

---

# 8. Pruebas funcionales BT

### BT-01 — `team`, n=3

```text
nodos_visitados=1
nodos_podados=1
soluciones=0
```

Como `3 < 5`, la instancia es imposible desde la raíz.

**PASS.**

### BT-02 — `team`, n=4

```text
nodos_visitados=1
nodos_podados=1
soluciones=0
```

Como `4 < 5`, la instancia es imposible.

**PASS.**

### BT-03 — `team`, n=5

```text
nodos_visitados=56,389,826
nodos_podados=243,302,917
soluciones=51,916,800
```

Tiempo observado: aproximadamente 2.45–2.52 s.

**PASS.**

### BT-04 — Regresión `team`, n=3

Ejecutado después de integrar los cambios del repositorio:

```text
nodos_visitados=1
nodos_podados=1
soluciones=0
```

**PASS.**

### BT-05 — Regresión `relaxed`, n=3

Ejecutado después de integrar los cambios del repositorio:

```text
nodos_visitados=230742
nodos_podados=70089
soluciones=226252
tiempo_ms=3.9229
```

**PASS.**

---

# 9. Validación matemática `team`, n=5

Sin considerar inicialmente `noConsecutive`:

```text
C(5,2) × 3! × 26^3 × 10 × 5 = 52,728,000
```

Casos inválidos por las dos minúsculas iguales y consecutivas:

```text
4 × 6 × 26 × 26 × 10 × 5 = 811,200
```

Por tanto:

```text
52,728,000 - 811,200 = 51,916,800
```

El programa produjo exactamente `51,916,800`.

**PASS.**

---

# 10. Validación matemática `relaxed`, n=3

```text
67^3 = 300,763
41^3 = 68,921
300,763 - 68,921 = 231,842
```

El número de cadenas con al menos una minúscula es 231.842. Después de retirar las cadenas que violan la prohibición de consecutivos iguales:

```text
231,842 - 5,590 = 226,252
```

El programa produjo `226,252`.

**PASS.**

---

# 11. Comparación con y sin poda

### `relaxed`, n=3

```text
árbol completo = 305,320
con poda       = 230,742
reducción      = 24.4262 %
soluciones     = 226,252
```

### `none`, n=3

```text
árbol completo = 305,320
con poda       = 296,342
reducción      = 2.94052 %
soluciones     = 291,852
```

### `team`, n=3

```text
árbol completo = 305,320
con poda       = 1
reducción      = 99.9997 %
soluciones     = 0
```

### `relaxed`, n=4

```text
árbol completo = 20,456,441
con poda       = 16,934,574
reducción      = 17.2164 %
soluciones     = 16,638,232
```

### `team`, n=5

```text
árbol completo = 1,370,581,548
con poda       = 56,389,826
reducción      = 95.8857 %
soluciones     = 51,916,800
```

Tiempo observado para `team`, n=5:

```text
con poda    ≈ 2.55 s
sin poda    ≈ 37.00 s
```

Relación aproximada:

```text
37.00 / 2.55 ≈ 14.5
```

En las comparaciones realizadas, `soluciones_iguales=1`.

---

# 12. Tabla consolidada de comparación BT: con poda vs. sin poda

El enunciado exige comparar, para las mismas instancias, la exploración exhaustiva sin poda frente a la versión con poda. Los resultados obtenidos fueron:

| Política | n | Nodos árbol completo | Nodos con poda | Nodos podados | Reducción del espacio | Soluciones con poda | Soluciones sin poda | Tiempo con poda | Tiempo sin poda |
|:---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| `none` | 3 | 305,320 | 296,342 | 4,489 | 2.94052 % | 291,852 | 291,852 | 4.8496 ms | 5.2484 ms |
| `relaxed` | 3 | 305,320 | 230,742 | 70,089 | 24.4262 % | 226,252 | 226,252 | 6.7236 ms | 7.896 ms |
| `team` | 3 | 305,320 | 1 | 1 | 99.9997 % | 0 | 0 | 0.0001 ms | 6.4083 ms |
| `relaxed` | 4 | 20,456,441 | 16,934,574 | 2,920,341 | 17.2164 % | 16,638,232 | 16,638,232 | 284.8 ms | 475.941 ms |
| `team` | 5 | 1,370,581,548 | 56,389,826 | 243,302,917 | 95.8857 % | 51,916,800 | 51,916,800 | 2,545.83 ms | 37,003.7 ms |

### Interpretación

En todas las comparaciones realizadas:

```text
soluciones_iguales=1
```

Por tanto, la poda reduce el espacio de búsqueda sin modificar el número de soluciones encontradas.

El caso `team, n=5` presenta la mayor reducción observada:

```text
95.8857 %
```

Además:

```text
37,003.7 / 2,545.83 ≈ 14.5
```

por lo que la versión con poda fue aproximadamente 14.5 veces más rápida que la exploración sin poda en esta instancia.

El caso `team, n=3` muestra la poda más extrema: como la política exige al menos 5 caracteres, la imposibilidad se detecta directamente en la raíz.

### Datos de regresión

Después de integrar los cambios del repositorio se volvieron a ejecutar instancias representativas de BT:

| Política | n | Nodos visitados | Nodos podados | Soluciones | Tiempo |
|:---:|---:|---:|---:|---:|---:|
| `team` | 3 | 1 | 1 | 0 | 0.0001 ms |
| `relaxed` | 3 | 230,742 | 70,089 | 226,252 | 3.9229 ms |

Estos datos corresponden a pruebas de regresión y no sustituyen las mediciones de la tabla comparativa anterior.

---

# 14. BT n=6 y costo computacional

### `team`, n=6

```text
nodos_visitados = 9,454,738,424
nodos_podados   = 28,794,330,385
soluciones      = 8,883,856,800
tiempo_ms       = 345,757
```

```text
345.76 s ≈ 5.76 min
```

### `none`, n=6

```text
nodos_visitados = 85,197,148,478
nodos_podados   = 1,290,865,885
soluciones      = 83,906,282,592
tiempo_ms       = 1,388,040
```

```text
≈ 23.13 min
```

Esto evidencia el crecimiento exponencial y justifica no repetir indiscriminadamente estas instancias costosas.

---

# 14. Tests automatizados

### `tests/test_fb.py`

Verifica hash de referencia, `verify-ref`, búsqueda directa, fronteras de búsqueda y generación determinista de las instancias.

Ejecutar:

```powershell
python tests/test_fb.py
```

### `tests/test_bt.py`

Verifica `none`, `relaxed`, parámetros de `team`, conteos y equivalencia de soluciones con y sin poda.

Ejecutar:

```powershell
python tests/test_bt.py
```

Ninguno de estos tests requiere modificar `src/main.cpp`: utilizan la CLI existente.

---

# 15. Observación sobre contadores BT

En algunas ejecuciones:

```text
nodos_visitados + nodos_podados != nodos_arbol_completo
```

Por ejemplo:

```text
230,742 + 70,089 = 300,831
```

frente a:

```text
305,320
```

Esto no se clasifica como FAIL sin revisar la definición exacta de los contadores en `bt_backtracking.hpp`. Puede existir una diferencia entre nodos visitados, terminales y ramas podadas.

---

# 16. Complejidad

## FB

Para un alfabeto `|Σ|` y longitud `n`:

```text
|Σ|^n
```

candidatos.

Tiempo:

```text
O(|Σ|^n)
```

bajo la consideración habitual de costo constante del hash para las longitudes de la práctica.

Espacio auxiliar:

```text
O(n)
```

## BT

El peor caso continúa siendo exponencial. El árbol completo contiene:

```text
Σ(k=0..n) |Σ|^k
```

nodos.

La ventaja es que las restricciones permiten descartar ramas antes de completarlas.

La profundidad recursiva es `O(n)`, sin contar almacenamiento explícito de soluciones.

---

# 17. Hallazgos positivos

## FB

- Orden de enumeración validado en fronteras.
- A1 y A2 funcionan en los casos probados.
- La instancia `abc12` se recupera correctamente.
- El diccionario encuentra `access123`.
- La entrada de alfabeto inválido se maneja mediante un error controlado.
- Las excepciones de CLI no provocan terminación abrupta del proceso.

## BT

- Las políticas se interpretan correctamente.
- Los casos imposibles se podan desde la raíz.
- `team`, n=5 coincide con el conteo independiente.
- `relaxed`, n=3 coincide con el conteo independiente.
- La poda no cambia el número de soluciones en las comparaciones realizadas.
- `team`, n=5 reduce los nodos visitados en 95.8857%.

---

# 18. Defectos encontrados

### CLI-01 — Validación del hash

El programa acepta hashes como:

```text
abc
xyz
1234567890
```

y ejecuta la búsqueda.

También acepta un valor de 64 caracteres compuesto por `G`, aunque `G` no es hexadecimal.

**Estado: FAIL.**

### CLI-02 — Longitud cero

`length=0` es aceptado y produce un candidato.

**Estado: FAIL.**

### CLI-03 — Mensaje de error para longitud negativa

La entrada:

```text
--length -1
```

produce:

```text
[ERROR] cannot create std::vector larger than max_size()
```

**Estado: PASS.**

---

# 19. Estado de cierre QA

| Área | Estado |
|---|---|
| FB funcional | **PASS** |
| FB diccionario | **PASS** |
| FB robustez — alfabeto inválido | **PASS** |
| FB robustez — hash inválido | **FAIL** |
| FB robustez — length=0 | **FAIL** |
| FB robustez — length=-1 | **PASS** |
| BT funcional | **PASS** |
| BT políticas | **PASS** |
| BT conteos | **PASS** |
| BT poda vs. sin poda | **PASS** |
| Tests automatizados | **PASS** |
| BT n=6 | **PASS** |
| Documentación QA | **PASS** |

---

# 20. Conclusión

Las pruebas muestran que la lógica principal de FB y BT se comporta correctamente en las instancias funcionales y combinatorias verificadas.

FB presenta el crecimiento esperado de `|Σ|^n`; BT conserva el carácter exponencial en el peor caso, pero las restricciones pueden reducir significativamente el espacio explorado.

El resultado experimental más importante de BT es `team`, n=5: la poda reduce los nodos visitados en 95.8857% y mantiene exactamente 51.916.800 soluciones.

El manejo de excepciones incorporado en la CLI permite controlar entradas inválidas como alfabetos desconocidos y longitudes negativas sin terminación abrupta.

Los principales problemas pendientes corresponden a la validación del formato del hash y a la aceptación de `length=0`.

---

# 21. Evidencia de regresión posterior a la integración

Después de integrar los cambios del repositorio se recompiló el proyecto y se ejecutaron nuevamente casos representativos de FB, diccionario y BT.

Resultados:

```text
verify-ref → PASS
alphabet a3 → PASS
length -1 → PASS
hash abc → FAIL
hash xyz → FAIL
hash 1234567890 → FAIL
hash de 64 caracteres con G → FAIL
access123 en diccionario → PASS
BT team n=3 → PASS
BT relaxed n=3 → PASS
```

Los resultados experimentales de BT de tamaños grandes no se repitieron debido a su elevado costo computacional y se conservan como evidencia de las ejecuciones originales.
