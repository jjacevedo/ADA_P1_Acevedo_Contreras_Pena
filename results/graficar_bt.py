import matplotlib.pyplot as plt

# Datos obtenidos experimentalmente
team_n = [3, 4, 5, 6]
team_time = [0.0001, 0.0001, 2.54583, 345.757]

relaxed_n = [1, 3, 4]
relaxed_time = [0.0062, 6.7236, 284.8]

none_n = [1, 3, 6]
none_time = [0.0059, 4.8496, 1388.04]

plt.figure(figsize=(10, 6))

plt.plot(team_n, team_time, marker="o", label="team")
plt.plot(relaxed_n, relaxed_time, marker="o", label="relaxed")
plt.plot(none_n, none_time, marker="o", label="none")

plt.xlabel("Longitud n")
plt.ylabel("Tiempo de ejecución (ms)")
plt.title("Backtracking: tiempo de ejecución vs. longitud")
plt.legend()
plt.grid(True)

plt.tight_layout()
plt.savefig("results/bt_tiempo_vs_n.png", dpi=300)
plt.show()