#!/usr/bin/env bash
#
# clang-tidy lint — single source of truth for CI and local (IDE) runs.
# Mirrors the clang-tidy job in .gitlab-ci.yml / .github/workflows/lint.yml.
#
# Env overrides:
#   SCL_SRC_DIR    source directory to scan (default: src)
#   CLANG_TIDY     clang-tidy executable (default: clang-tidy on PATH)
#   SCL_LINT_JOBS  headers to scan at once (default: one per core)
#
set -euo pipefail
cd "$(dirname "$0")/../.."

SCL_SRC_DIR="${SCL_SRC_DIR:-src}"
CLANG_TIDY="${CLANG_TIDY:-clang-tidy}"
SCL_LINT_JOBS="${SCL_LINT_JOBS:-$(getconf _NPROCESSORS_ONLN 2> /dev/null || echo 2)}"

[ -d "$SCL_SRC_DIR" ] || { echo "No sources ($SCL_SRC_DIR), skipping."; exit 0; }

# A header is a whole translation unit to clang-tidy, and they share nothing, so the run
# costs one core's time divided by the cores there are. Findings of two headers can
# interleave; every diagnostic line carries the file it belongs to.
find "$SCL_SRC_DIR" \( -name '*.h' -o -name '*.hpp' \) -print0 |
    xargs -0 -r -P "$SCL_LINT_JOBS" -I{} \
        "$CLANG_TIDY" {} --quiet --warnings-as-errors='*' -- \
        -std=c++20 -xc++ -I"$SCL_SRC_DIR" || exit 1
