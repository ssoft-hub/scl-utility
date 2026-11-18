#!/usr/bin/env bash
#
# cppcheck lint — single source of truth for CI and local (IDE) runs.
# Run by the cppcheck job in .gitlab-ci.yml and in .github/workflows/lint.yml.
# Uses the project .cppcheck suppressions list.
#
# The analysis of every header is kept in a build directory between runs, so a run checks
# again only the headers whose code, or the code of whose includes, has changed. With -j,
# cppcheck also runs its checks across headers only when it has a build directory.
#
# Env overrides:
#   SCL_SRC_DIR       source directory to scan (default: src)
#   CPPCHECK          cppcheck executable (default: cppcheck on PATH)
#   SCL_LINT_JOBS     headers to scan at once (default: one per core the run may use)
#   SCL_CPPCHECK_DIR  directory kept between runs, holding the key-* directories of this
#                     script (default: build/lint/cppcheck)
#
set -euo pipefail
cd "$(dirname "$0")/../.."

# shellcheck source=script/lint/usable_cores.sh
source "script/lint/usable_cores.sh"

SCL_SRC_DIR="${SCL_SRC_DIR:-src}"
CPPCHECK="${CPPCHECK:-cppcheck}"
SCL_LINT_JOBS="${SCL_LINT_JOBS:-$(usable_cores)}"
SCL_CPPCHECK_DIR="${SCL_CPPCHECK_DIR:-build/lint/cppcheck}"

[ -d "$SCL_SRC_DIR" ] || { echo "No sources ($SCL_SRC_DIR), skipping."; exit 0; }

SUPP_LIST=$(mktemp)
trap 'rm -f "$SUPP_LIST"' EXIT
grep -v '^\s*#' .cppcheck | grep -v '^\s*$' | tr -d '\r' > "$SUPP_LIST"

options=(
    --enable=warning,style,performance,portability
    --check-level=exhaustive
    --max-configs=32
    --std=c++20
    --language=c++
    --inline-suppr
    -I"$SCL_SRC_DIR"
    -USCL_DOXYGEN
)

# A result kept under other options, another suppressions list or another cppcheck can
# differ from the one a fresh run gives, so each of them gets a directory of its own, and
# the other `key-*` directories in it are removed.
key=$({ "$CPPCHECK" --version; printf '%s\n' "${options[@]}"; cat "$SUPP_LIST"; } | cksum)
key="key-${key%% *}"
mkdir -p "$SCL_CPPCHECK_DIR/$key"
find "$SCL_CPPCHECK_DIR" -mindepth 1 -maxdepth 1 -type d -name 'key-*' ! -name "$key" -exec rm -rf {} +

find "$SCL_SRC_DIR" \( -name '*.h' -o -name '*.hpp' \) -exec "$CPPCHECK" \
    "${options[@]}" \
    -j "$SCL_LINT_JOBS" \
    --cppcheck-build-dir="$SCL_CPPCHECK_DIR/$key" \
    --error-exitcode=1 \
    --suppressions-list="$SUPP_LIST" \
    {} +
