#!/usr/bin/env python3
# Genera results/fb_vs_diccionario.png a partir de results/comparacion_fb_diccionario.csv.
# Excluye la fila "control" (no tiene comparación FB real, ver README).
import csv
import matplotlib.pyplot as plt

rows = [r for r in csv.DictReader(open("results/comparacion_fb_diccionario.csv"))
        if r["alfabeto"] != "control"]

labels = [f"{r['alfabeto'].upper()} n={r['n']}" for r in rows]
fb_ms = [float(r["tiempo_fb_ms"]) for r in rows]
dict_ms = [float(r["tiempo_dict_ms"]) for r in rows]
speedup = [fb / d for fb, d in zip(fb_ms, dict_ms)]

x = range(len(labels))
fig, ax = plt.subplots(figsize=(9, 6.5))
ax.bar(x, fb_ms, color="#1f77b4", label="Fuerza bruta")
ax.bar(x, dict_ms, color="#ff7f0e", label="Diccionario")
ax.set_yscale("log")
ax.set_xticks(list(x))
ax.set_xticklabels(labels)
ax.set_ylabel("Tiempo de ejecución (ms, escala log)")
ax.set_title("Módulo FB — Fuerza bruta vs. diccionario (mismas 5 instancias)")
ax.legend()

for i, s in enumerate(speedup):
    ax.text(i, fb_ms[i] * 1.15, f"×{s:,.0f}", ha="center", fontsize=10, color="#333")

fig.tight_layout()
fig.savefig("results/fb_vs_diccionario.png", dpi=120)
print("Escrito: results/fb_vs_diccionario.png")
