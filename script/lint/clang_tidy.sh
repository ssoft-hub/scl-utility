#!/usr/bin/env bash
#
# clang-tidy lint — single source of truth for CI and local (IDE) runs.
# Run by the clang job in .gitlab-ci.yml and the clang-tidy job in
# .github/workflows/lint.yml.
#
# Env overrides:
#   SCL_SRC_DIR    source directory to scan (default: src)
#   CLANG_TIDY     clang-tidy executable (default: clang-tidy on PATH)
#   SCL_LINT_JOBS  headers to scan at once (default: one per core the run may use)
#
set -euo pipefail
cd "$(dirname "$0")/../.."

# The cores the run may use. A container reads the count the machine has rather than the
# share it was given, so a cgroup quota, where one is set, is what bounds the scan.
usable_cores() {
    local online quota period
    online=$(getconf _NPROCESSORS_ONLN 2> /dev/null || echo 2)

    if [ -r /sys/fs/cgroup/cpu.max ]; then                    # cgroup v2
        read -r quota period < /sys/fs/cgroup/cpu.max || true
    elif [ -r /sys/fs/cgroup/cpu/cpu.cfs_quota_us ]; then     # cgroup v1
        quota=$(cat /sys/fs/cgroup/cpu/cpu.cfs_quota_us)
        period=$(cat /sys/fs/cgroup/cpu/cpu.cfs_period_us)
    fi

    # An unset quota reads `max` under v2 and `-1` under v1, and neither bounds anything.
    case "${quota:-max}${period:-x}" in *[!0-9]*) echo "$online"; return ;; esac
    [ "$period" -gt 0 ] || { echo "$online"; return; }

    quota=$((quota / period))
    [ "$quota" -ge 1 ] || quota=1
    if [ "$quota" -lt "$online" ]; then echo "$quota"; else echo "$online"; fi
}

SCL_SRC_DIR="${SCL_SRC_DIR:-src}"
CLANG_TIDY="${CLANG_TIDY:-clang-tidy}"
SCL_LINT_JOBS="${SCL_LINT_JOBS:-$(usable_cores)}"

[ -d "$SCL_SRC_DIR" ] || { echo "No sources ($SCL_SRC_DIR), skipping."; exit 0; }

# A header is a whole translation unit to clang-tidy, and they share nothing, so the run
# costs one core's time divided by the cores there are. Findings of two headers
# interleave, each diagnostic naming the file it belongs to.
find "$SCL_SRC_DIR" \( -name '*.h' -o -name '*.hpp' \) -print0 |
    xargs -0 -r -P "$SCL_LINT_JOBS" -I{} \
        "$CLANG_TIDY" {} --quiet --warnings-as-errors='*' -- \
        -std=c++20 -xc++ -I"$SCL_SRC_DIR" || exit 1
