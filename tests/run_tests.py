#!/usr/bin/env python3
from __future__ import annotations

import argparse
import subprocess
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


def _bytes_label(data: bytes) -> str:
    if not data:
        return "<empty>"

    try:
        return repr(data.decode("utf-8"))
    except UnicodeDecodeError:
        return "0x" + data.hex()


def _read_exit_code(path: Path) -> int:
    if not path.exists():
        return 0

    text = path.read_text(encoding="utf-8").strip()
    return int(text) if text else 0


def _collect_cases(
    tests_dir: Path,
    names: set[str],
    prefixes: list[str],
    exclude_prefixes: list[str],
) -> list[Path]:
    cases = sorted(tests_dir.glob("*.brfkpp"))
    if prefixes:
        cases = [case for case in cases if any(case.stem.startswith(prefix) for prefix in prefixes)]
    if exclude_prefixes:
        cases = [case for case in cases if not any(case.stem.startswith(prefix) for prefix in exclude_prefixes)]
    if names:
        cases = [case for case in cases if case.stem in names or case.name in names]
    return cases


def _build(make: str, target: str) -> None:
    subprocess.run([make, target], cwd=ROOT, check=True)


def _run_case(binary: Path, source: Path) -> tuple[bool, str]:
    expected_path = source.with_suffix(".out")
    input_path = source.with_suffix(".in")
    exit_path = source.with_suffix(".exit")

    if not expected_path.exists():
        return False, f"{source.name}: missing expected output {expected_path.name}"

    stdin = input_path.read_bytes() if input_path.exists() else b""
    expected_stdout = expected_path.read_bytes()
    expected_code = _read_exit_code(exit_path)

    proc = subprocess.run(
        [str(binary), str(source)],
        cwd=ROOT,
        input=stdin,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )

    ok = proc.returncode == expected_code and proc.stdout == expected_stdout
    if ok:
        return True, f"{source.name}: ok"

    details = [
        f"{source.name}: failed",
        f"  exit: expected {expected_code}, got {proc.returncode}",
        f"  stdout: expected {_bytes_label(expected_stdout)}, got {_bytes_label(proc.stdout)}",
    ]
    if proc.stderr:
        details.append(f"  stderr: {_bytes_label(proc.stderr)}")
    return False, "\n".join(details)


def main() -> int:
    parser = argparse.ArgumentParser(description="Run Brainfuck++ interpreter tests")
    parser.add_argument("--binary", default=str(ROOT / "out-cpl"), help="Interpreter binary path")
    parser.add_argument("--tests-dir", default=str(ROOT / "tests"), help="Directory with .brfkpp cases")
    parser.add_argument("--no-build", action="store_true", help="Skip make before running tests")
    parser.add_argument("--make", default="make", help="Make command")
    parser.add_argument("--build-target", default="all", help="Make target used before tests")
    parser.add_argument("--prefix", action="append", default=[], help="Only run cases whose stem starts with this prefix")
    parser.add_argument("--exclude-prefix", action="append", default=[], help="Skip cases whose stem starts with this prefix")
    parser.add_argument("cases", nargs="*", help="Optional case stems or filenames to run")
    args = parser.parse_args()

    binary = Path(args.binary)
    if not binary.is_absolute():
        binary = ROOT / binary

    tests_dir = Path(args.tests_dir)
    if not tests_dir.is_absolute():
        tests_dir = ROOT / tests_dir

    if not args.no_build:
        _build(args.make, args.build_target)

    cases = _collect_cases(tests_dir, set(args.cases), args.prefix, args.exclude_prefix)
    if not cases:
        print("No tests found", file=sys.stderr)
        return 1

    failed = 0
    for case in cases:
        ok, message = _run_case(binary, case)
        print(message)
        failed += 0 if ok else 1

    if failed:
        print(f"\n{failed}/{len(cases)} tests failed", file=sys.stderr)
        return 1

    print(f"\n{len(cases)} tests passed")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
