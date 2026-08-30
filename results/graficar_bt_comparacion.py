import matplotlib.pyplot as plt

casos = [
    "none n=3",
    "relaxed n=3",
    "team n=3",
    "relaxed n=4",
    "team n=5"
]

tiempo_poda = [
    4.8496,
    6.7236,
    0.0001,
    284.8,
    2545.83
]

tiempo_sin_poda = [
    5.2484,
    7.896,
    6.4083,
    475.941,
    37003.7
]

x = range(len(casos))

plt.figure(figsize=(11, 6))

plt.bar(
    [i - 0.2 for i in x],
    tiempo_poda,
    width=0.4,
    label="Con poda"
)

plt.bar(
    [i + 0.2 for i in x],
    tiempo_sin_poda,
    width=0.4,
    label="Sin poda"
)

plt.xticks(x, casos, rotation=20)
plt.xlabel("Configuración")
plt.ylabel("Tiempo de ejecución (ms)")
plt.title("Backtracking: comparación de tiempo con y sin poda")
plt.legend()
plt.grid(axis="y")

plt.tight_layout()
plt.savefig("results/bt_poda_vs_sin_poda.png", dpi=300)
plt.show()