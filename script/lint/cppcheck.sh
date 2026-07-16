#!/usr/bin/env bash
#
# cppcheck lint — single source of truth for CI and local (IDE) runs.
# Mirrors the cppcheck job in .gitlab-ci.yml / .github/workflows/lint.yml.
# Uses the project .cppcheck suppressions list.
#
# Env overrides:
#   SCL_SRC_DIR   source directory to scan (default: src)
#   CPPCHECK      cppcheck executable (default: cppcheck on PATH)
#
set -euo pipefail
cd "$(dirname "$0")/../.."

SCL_SRC_DIR="${SCL_SRC_DIR:-src}"
CPPCHECK="${CPPCHECK:-cppcheck}"

[ -d "$SCL_SRC_DIR" ] || { echo "No sources ($SCL_SRC_DIR), skipping."; exit 0; }

SUPP_LIST=$(mktemp)
trap "rm -f '$SUPP_LIST'" EXIT
grep -v '^\s*#' .cppcheck | grep -v '^\s*$' | tr -d '\r' > "$SUPP_LIST"

find "$SCL_SRC_DIR" \( -name '*.h' -o -name '*.hpp' \) -exec "$CPPCHECK" \
    --enable=warning,style,performance,portability \
    --check-level=exhaustive \
    --std=c++20 \
    --language=c++ \
    --inline-suppr \
    --error-exitcode=1 \
    --suppressions-list="$SUPP_LIST" \
    -I"$SCL_SRC_DIR" \
    -UDOXYGEN \
    {} +
