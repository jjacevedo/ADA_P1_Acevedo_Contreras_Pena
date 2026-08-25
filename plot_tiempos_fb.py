#!/usr/bin/env python3
# Genera results/tiempo_vs_n_fb.png a partir de results/tiempos_fb.csv (peor caso).
import csv
import matplotlib.pyplot as plt

rows = list(csv.DictReader(open("results/tiempos_fb.csv")))
worst = [r for r in rows if r["tipo_caso"] == "peor_caso"]

def serie(alf):
    pts = sorted((int(r["n"]), float(r["tiempo_ms"])) for r in worst if r["alfabeto"] == alf)
    return [p[0] for p in pts], [p[1] for p in pts]

n1, t1 = serie("a1")
n2, t2 = serie("a2")

n_teorico = list(range(n1[0], n1[-1] + 1))
base_n, base_t = n1[0], t1[0]
t_teorico = [base_t * (26 ** (n - base_n)) for n in n_teorico]

fig, ax = plt.subplots(figsize=(9, 6.5))
ax.plot(n1, t1, "o-", color="#1f77b4", label="A1 (26 símbolos) — medido", linewidth=2, markersize=8)
ax.plot(n2, t2, "o-", color="#ff7f0e", label="A2 (36 símbolos) — medido", linewidth=2, markersize=8)
ax.plot(n_teorico, t_teorico, "--", color="gray", label="Teórico 26ⁿ (escalado a n=3)")
ax.set_yscale("log")
ax.set_xlabel("Longitud de la contraseña (n)")
ax.set_ylabel("Tiempo de ejecución (ms, escala log)")
ax.set_title("Módulo FB — Peor caso: tiempo medido vs. crecimiento teórico")
ax.set_xticks(sorted(set(n1 + n2)))
ax.grid(True, which="both", alpha=0.3)
ax.legend()
fig.tight_layout()
fig.savefig("results/tiempo_vs_n_fb.png", dpi=120)
print("Escrito: results/tiempo_vs_n_fb.png")
