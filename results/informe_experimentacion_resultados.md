# Módulo FB — Experimentación y Resultados (para pegar en el Informe)

Basado en los CSV congelados: `results/tiempos_fb.csv` y `results/comparacion_fb_diccionario.csv` (commit `604cedb`).

---

## Tabla 1 — Crecimiento de FB por n y alfabeto (peor caso)

| Alfabeto | n | \|Σ\|ⁿ (teórico) | Candidatos evaluados | Tiempo | Factor medido | Factor teórico |
|---|---|---|---|---|---|---|
| A1 | 3 | 17,576 | 17,576 | 38.2 ms | — | ×26 |
| A1 | 4 | 456,976 | 456,976 | 556.8 ms | ×14.6 | ×26 |
| A1 | 5 | 11,881,376 | 11,881,376 | 14.76 s | ×26.5 | ×26 |
| A1 | 6 | 308,915,776 | 308,915,776 | 11.39 min | ×46.3 | ×26 |
| A2 | 3 | 46,656 | 46,656 | 56.8 ms | — | ×36 |
| A2 | 4 | 1,679,616 | 1,679,616 | 2.09 s | ×36.8 | ×36 |
| A2 | 5 | 60,466,176 | 60,466,176 | 1.25 min | ×35.9 | ×36 |

En las 7 configuraciones, los candidatos evaluados coinciden **exactamente** con la cota teórica |Σ|ⁿ — evidencia de que la enumeración no omite ni repite candidatos.

## Tabla 2 — Instancias propias del equipo (semilla = 2124, Acevedo/Contreras/Peña)

Orden A1,A2,A1,A2,A1 confirmado oficialmente por el profesor.

| # | n | Alfabeto | Contraseña | Candidatos hasta encontrarla | Tiempo | % del espacio recorrido |
|---|---|---|---|---|---|---|
| 1 | 4 | A1 | `slaz` | 323,830 | 393.7 ms | 70.9% de 456,976 |
| 2 | 4 | A2 | `y70p` | 1,163,464 | 1.40 s | 69.3% de 1,679,616 |
| 3 | 5 | A1 | `ixuxs` | 4,074,193 | 5.03 s | 34.3% de 11,881,376 |
| 4 | 5 | A2 | `bgdaf` | 1,963,446 | 2.42 s | 3.2% de 60,466,176 |
| 5 | 6 | A1 | `ixmbgr` | 105,773,218 | 4.05 min | 34.2% de 308,915,776 |

(% del espacio recorrido = candidatos_evaluados / |Σ|ⁿ — ilustra que el punto donde el LCG coloca la contraseña dentro del espacio es esencialmente arbitrario: en la instancia 4 se encontró casi al principio, en la 3 y la 5 a un tercio del camino.)

## Tabla 3 — Fuerza bruta vs. ataque por diccionario

| Instancia | n | Tiempo FB | Tiempo diccionario | Speedup diccionario | ¿Estaba en diccionario.txt? |
|---|---|---|---|---|---|
| A1 n=4 (`slaz`) | 4 | 390.0 ms | 0.99 ms | ×393 | No |
| A2 n=4 (`y70p`) | 4 | 1.42 s | 0.69 ms | ×2,063 | No |
| A1 n=5 (`ixuxs`) | 5 | 4.99 s | 0.66 ms | ×7,569 | No |
| A2 n=5 (`bgdaf`) | 5 | 2.45 s | 0.66 ms | ×3,713 | No |
| A1 n=6 (`ixmbgr`) | 6 | 4.05 min | 1.33 ms | ×182,427 | No |
| control (`acceso123`) | 9 | n/a (26⁹ intratable) | 0.02 ms | — | **Sí** |

## Tabla 4 — Tasa de éxito

| Estrategia | Instancias probadas | Encontradas | Tasa de éxito |
|---|---|---|---|
| Fuerza bruta (sobre las 5 instancias reales) | 5 | 5 | 100% |
| Diccionario (sobre las 5 instancias reales) | 5 | 0 | 0% |
| Diccionario (incluyendo el control) | 6 | 1 | 16.7% |

---

## Análisis de resultados

**Crecimiento observado vs. O(|Σ|ⁿ).** Las 7 configuraciones evaluaron exactamente |Σ|ⁿ candidatos, sin excepción, confirmando que la implementación cubre el espacio completo sin omisiones ni repeticiones. Los factores de crecimiento medidos (×26.5 y ×36.8/×35.9 al pasar de un n al siguiente, para A1 y A2 respectivamente) coinciden casi exactamente con la cota teórica |Σ| — evidencia directa de que el algoritmo se comporta como O(|Σ|ⁿ), tal como predice el análisis de la Sección 6.1. Las dos desviaciones que sí aparecen tienen explicación, no son errores: en n=3 el factor medido es más bajo de lo esperado (×14.6 en vez de ×26) porque a esa escala el tiempo total (38–56 ms) es comparable al overhead fijo del proceso (arranque, buffers de E/S), que no escala con |Σ|ⁿ; y en A1 n=6 el factor es más alto de lo esperado (×46.3) porque el *throughput* (candidatos por segundo) cae de ~805,000–820,000 cand/s en las corridas cortas a 452,000 cand/s en la corrida de 11.4 minutos — una caída del 45% consistente con *throttling* térmico del procesador bajo carga sostenida en un MacBook Air sin ventilador activo, no con un cambio en el algoritmo.

**Diferencia entre A1 y A2.** A2 tiene 36 símbolos contra 26 de A1 (38% más), y esa diferencia se refleja directamente en el tiempo: para el mismo n, A2 siempre tarda más que A1 (ej. n=4: 556.8 ms vs. 2.09 s; n=5: 14.76 s vs. 1.25 min). El factor entre A2 y A1 al mismo n debería acercarse a (36/26)ⁿ; para n=5 eso da (36/26)⁵ ≈ 5.2, y lo medido es 75.0/14.76 ≈ 5.08 — nuevamente consistente con la teoría.

**Efecto de aumentar n.** Cada unidad adicional de n multiplica el costo por |Σ|, no lo suma — es la esencia del "muro exponencial" que describe la Sección 4 del enunciado. Pasar de n=3 a n=6 en A1 no cuadruplica el tiempo, lo multiplica por ~18,000 (de 38 ms a 11.4 min).

**Punto en que FB deja de ser práctico (en esta Mac).** Con throughput sostenido de ~450,000–820,000 candidatos por segundo, A1 con n=6 (309 millones de candidatos) ya exige más de 11 minutos para un solo hash objetivo. A1 con n=7 (Σ=26) tendría ~8,000 millones de candidatos — a este ritmo, entre 2.7 y 5 horas por hash, ya inviable para explorar en una sesión de trabajo normal. Ese es el punto concreto que marcamos como el límite práctico de la fuerza bruta pura en este equipo de cómputo.

**Ventajas y limitaciones del diccionario.** La ventaja es velocidad: entre 393 y 182,427 veces más rápido que la fuerza bruta en las mismas instancias, porque nunca genera candidatos — solo evalúa los 500 ya escritos en la lista. La limitación es que **no es exhaustivo**: en las 5 instancias reales del equipo (generadas por LCG, no palabras "comunes"), la tasa de éxito fue 0% — ninguna estaba en la lista, así que el ataque terminó en "no encontrada" en cada caso, aunque la fuerza bruta sí encontró las 5. El caso de control (`acceso123`, sí está en la lista) muestra el otro extremo: éxito en 0.02 ms. La conclusión que sostienen los datos: el diccionario es una heurística de "primer intento" — rápida y útil cuando la contraseña sigue patrones comunes, pero sin ninguna garantía de completitud, a diferencia de la fuerza bruta que siempre encuentra la contraseña si esta existe dentro del espacio explorado.
