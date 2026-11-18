#!/usr/bin/env bash
#
# clang-format lint — single source of truth for CI and local (IDE) runs.
# Run by the clang job in .gitlab-ci.yml and the clang-format job in
# .github/workflows/lint.yml.
#
# Scans the sources as well as the headers, and the test, example and benchmark
# trees as well as src: formatting is checked over everything a contributor writes,
# so a tree nobody scans cannot drift away from .clang-format unnoticed.
#
# The lint jobs run clang-format 22, and a local run wants the same: 21 formats a
# `is_same_v<X &&, ...>` in the test tree differently from every version around it.
#
# Env overrides:
#   SCL_FORMAT_DIRS  directories to scan, space-separated (default: src test example benchmark)
#   CLANG_FORMAT     clang-format executable (default: clang-format on PATH)
#   SCL_LINT_JOBS    clang-format processes at once (default: one per core the run may use)
#
set -euo pipefail
cd "$(dirname "$0")/../.."

# shellcheck source=script/lint/usable_cores.sh
source "script/lint/usable_cores.sh"

SCL_FORMAT_DIRS="${SCL_FORMAT_DIRS:-src test example benchmark}"
CLANG_FORMAT="${CLANG_FORMAT:-clang-format}"
SCL_LINT_JOBS="${SCL_LINT_JOBS:-$(usable_cores)}"

dirs=()
for dir in $SCL_FORMAT_DIRS; do
    [ -d "$dir" ] && dirs+=("$dir")
done

[ "${#dirs[@]}" -ne 0 ] || { echo "No sources ($SCL_FORMAT_DIRS), skipping."; exit 0; }

# Without -n, xargs puts as many files into one command as the command-line limit allows,
# so most of them go to one process.
find "${dirs[@]}" \( -name '*.h' -o -name '*.hpp' -o -name '*.cpp' -o -name '*.cc' \) \
    | xargs --no-run-if-empty -P "$SCL_LINT_JOBS" -n 16 "$CLANG_FORMAT" --dry-run --Werror
