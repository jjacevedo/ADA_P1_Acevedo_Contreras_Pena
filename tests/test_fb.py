#!/usr/bin/env python3
import os
import subprocess
import tempfile


ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def run(cmd, timeout=None):
    return subprocess.run(
        cmd,
        cwd=ROOT,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        check=True,
        timeout=timeout,
    )


def main():
    exe_name = "ada_fb_test.exe" if os.name == "nt" else "ada_fb_test"
    exe = os.path.join(tempfile.gettempdir(), exe_name)

    # El test compila el código existente.
    # No modifica src/main.cpp.
    run([
        "g++", "-std=c++17", "-O2", "-Wall",
        "-Isrc", "src/main.cpp", "-o", exe
    ])

    # ---------------------------------------------------------
    # 1. Hash de la instancia oficial abc12.
    # ---------------------------------------------------------

    out = run([exe, "hash", "abc12"])

    assert out.stdout.strip() == (
        "8d51feb34e3e69f6fa6dffc577e2c60490cf9a7fcd835f9f6af1505b71d74773"
    )

    # ---------------------------------------------------------
    # 2. verify-ref debe recuperar abc12 sobre A2, n=5.
    # ---------------------------------------------------------

    out = run([exe, "verify-ref"])

    assert "Encontrada: SI" in out.stdout
    assert "abc12" in out.stdout

    # ---------------------------------------------------------
    # 3. Búsqueda directa de la instancia de referencia.
    # ---------------------------------------------------------

    out = run([
        exe,
        "search",
        "--alphabet", "a2",
        "--length", "5",
        "--hash",
        "8d51feb34e3e69f6fa6dffc577e2c60490cf9a7fcd835f9f6af1505b71d74773",
    ])

    assert "encontrada=1" in out.stdout
    assert "password=abc12" in out.stdout

    # ---------------------------------------------------------
    # 4. Primera contraseña de A1, n=2:
    #    aa debe ser el candidato 1.
    # ---------------------------------------------------------

    import hashlib

    hash_aa = hashlib.sha256(b"aa").hexdigest()

    out = run([
        exe,
        "search",
        "--alphabet", "a1",
        "--length", "2",
        "--hash", hash_aa,
    ])

    assert "encontrada=1" in out.stdout
    assert "password=aa" in out.stdout
    assert "candidatos_evaluados=1" in out.stdout

    # ---------------------------------------------------------
    # 5. Última contraseña de A1, n=2:
    #    zz debe ser el candidato 676.
    # ---------------------------------------------------------

    hash_zz = hashlib.sha256(b"zz").hexdigest()

    out = run([
        exe,
        "search",
        "--alphabet", "a1",
        "--length", "2",
        "--hash", hash_zz,
    ])

    assert "encontrada=1" in out.stdout
    assert "password=zz" in out.stdout
    assert "candidatos_evaluados=676" in out.stdout

    # ---------------------------------------------------------
    # 6. Hash inexistente:
    #    debe recorrer todo A1^2 = 676 candidatos.
    # ---------------------------------------------------------

    out = run([
        exe,
        "search",
        "--alphabet", "a1",
        "--length", "2",
        "--hash",
        "0000000000000000000000000000000000000000000000000000000000000000",
    ])

    assert "encontrada=0" in out.stdout
    assert "candidatos_evaluados=676" in out.stdout

    # ---------------------------------------------------------
    # 7. Generación determinista de las instancias del equipo.
    # ---------------------------------------------------------

    out = run([
        exe,
        "gen-instances",
        "--apellidos", "acevedo,contreras,pena"
    ])

    assert "Semilla del equipo = 2124" in out.stdout

    assert (
        "4 | a1 | slaz | "
        "d477fe9fdf5725d7ff091744b9662732735b32165ba8978fa116f176b5a25dfc"
    ) in out.stdout

    assert (
        "4 | a2 | y70p | "
        "dc319045ebdcd8bd3af2e6459de2975233a05e371cd5e7ec3f8b88a5a43473a0"
    ) in out.stdout

    assert (
        "5 | a1 | ixuxs | "
        "c16a61c136f28aa6f95c87747ab2f4a13ab7106babf6f62db7873200960c8bae"
    ) in out.stdout

    assert (
        "5 | a2 | bgdaf | "
        "d6809ac7b6f13c1d90add5ef7fb3f2b8599b112809c50ed19251d9d68c14517f"
    ) in out.stdout

    assert (
        "6 | a1 | ixmbgr | "
        "70bebf0bcde96437c8891522b46033087e12f7bfad3421cd1f068b3ee820b8c1"
    ) in out.stdout

    print("[OK] Tests automatizados de FB ejecutados correctamente.")


if __name__ == "__main__":
    main()