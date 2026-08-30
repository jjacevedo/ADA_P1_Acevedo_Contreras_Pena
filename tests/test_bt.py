#!/usr/bin/env python3
import os
import subprocess
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

def run(cmd):
    return subprocess.run(
        cmd,
        cwd=ROOT,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        check=True,
    )

def values(text):
    result = {}
    for line in text.splitlines():
        if "=" in line:
            k, v = line.split("=", 1)
            result[k.strip()] = v.strip()
    return result

def main():
    # Este test usa n=3 para que la version sin poda sea ejecutable.
    # 67^3 = 300763 hojas.
    exe = os.path.join(tempfile.gettempdir(), "ada_bt_test")

    compile_cmd = [
        "g++", "-std=c++17", "-O2", "-Wall",
        "-Isrc", "src/main.cpp", "-o", exe
    ]

    run(compile_cmd)

    # 1. La politica sin restricciones de composicion (policy=none) NO
    #    tiene minimos por tipo de caracter, pero la prohibicion de
    #    caracteres consecutivos iguales SI aplica (Seccion 9.2 del PDF:
    #    "aplica a todas las instancias"). Por eso si debe podar, y el
    #    numero de soluciones es 67 * 66^(n-1), no 67^n.
    out = run([
        exe, "bt", "--length", "3",
        "--policy", "none", "--pruning", "on"
    ])
    v = values(out.stdout)

    assert v["sigma"] == "67"
    assert int(v["nodos_podados"]) > 0
    assert int(v["soluciones"]) == 67 * 66 * 66

    # 2. Con la politica relaxed se comprueba que poda y no poda
    #    encuentran exactamente las mismas soluciones.
    out_on = run([
        exe, "bt-compare", "--length", "3",
        "--policy", "relaxed", "--run-no-pruning", "1"
    ])
    v_on = values(out_on.stdout)

    assert v_on["soluciones_iguales"] == "1"
    assert int(v_on["nodos_con_poda"]) < int(v_on["nodos_arbol_completo"])
    assert float(v_on["reduccion_nodos_porcentaje"]) > 0.0

    # 3. La politica del equipo debe reconocer sus parametros.
    out_team = run([
        exe, "bt", "--length", "5",
        "--policy", "team"
    ])
    v_team = values(out_team.stdout)

    assert v_team["minLower"] == "2"
    assert v_team["minUpper"] == "1"
    assert v_team["minDigit"] == "1"
    assert v_team["minSymbol"] == "1"
    assert v_team["noConsecutive"] == "1"

    print("[OK] Tests de BT ejecutados correctamente.")

if __name__ == "__main__":
    main()
