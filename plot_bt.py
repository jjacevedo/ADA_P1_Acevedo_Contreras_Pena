import csv
import os
import matplotlib.pyplot as plt

CSV = "results/bt_comparacion.csv"
OUT = "results/bt_reduccion.png"

rows = []
if os.path.exists(CSV):
    with open(CSV, newline="", encoding="utf-8") as f:
        for row in csv.DictReader(f):
            if row.get("reduccion_porcentaje"):
                rows.append(row)

if not rows:
    print("No hay datos en", CSV)
    print("Ejecute primero run_bt_experiments.sh o agregue sus mediciones.")
    raise SystemExit(0)

labels = [r["caso"] for r in rows]
values = [float(r["reduccion_porcentaje"]) for r in rows]

plt.figure(figsize=(9, 5))
plt.bar(labels, values)
plt.ylabel("Reduccion del espacio (%)")
plt.xlabel("Configuracion")
plt.title("Backtracking: reduccion del espacio de busqueda")
plt.xticks(rotation=25, ha="right")
plt.tight_layout()
plt.savefig(OUT, dpi=150)
print("Grafica generada:", OUT)
