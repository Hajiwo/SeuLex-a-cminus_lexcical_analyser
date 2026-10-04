from pathlib import Path
import subprocess
import tempfile

scanner = Path(__file__).resolve().parents[1] / "build/scanner"
with tempfile.TemporaryDirectory() as directory:
    root = Path(directory)
    (root / "cminus.c").write_text("int integer;\n", encoding="ascii")
    subprocess.run([str(scanner)], cwd=root, check=True, timeout=10)
    tokens = (root / "tokenList.l").read_text()
    rows = [line.split() for line in tokens.splitlines()]
    assert [row[1] for row in rows] == ["int", "integer", ";"], tokens
    assert "Fundamental type" in tokens.splitlines()[0], tokens
    assert "Identifier" in tokens.splitlines()[1], tokens
print("Lexer smoke test passed")
