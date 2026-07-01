#!/usr/bin/env bash
#
# clang-tidy lint — single source of truth for CI and local (IDE) runs.
# Mirrors the clang-tidy job in .gitlab-ci.yml / .github/workflows/lint.yml.
#
# Env overrides:
#   SCL_SRC_DIR   source directory to scan (default: src)
#   CLANG_TIDY    clang-tidy executable (default: clang-tidy on PATH)
#
set -euo pipefail
cd "$(dirname "$0")/../.."

SCL_SRC_DIR="${SCL_SRC_DIR:-src}"
CLANG_TIDY="${CLANG_TIDY:-clang-tidy}"

[ -d "$SCL_SRC_DIR" ] || { echo "No sources ($SCL_SRC_DIR), skipping."; exit 0; }

find "$SCL_SRC_DIR" \( -name '*.h' -o -name '*.hpp' \) | while IFS= read -r f; do
    "$CLANG_TIDY" "$f" --quiet --warnings-as-errors='*' -- \
        -std=c++20 -xc++ -I"$SCL_SRC_DIR" || exit 1
done
