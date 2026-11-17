#!/usr/bin/env bash
#
# Run the whole lint stage (clang-format, clang-tidy, cppcheck, doxygen, doc snippets) the
# same way CI does. Each tool has its own script; this umbrella runs all five and aggregates
# the result. Intended for local (IDE) use — CI runs the same scripts, grouped into jobs
# by the image each one needs.
#
# The five scripts share no file, so they run at once, and the report of each one is
# printed after all of them, in the order below, whatever order they finish in. They run at
# the lowest priority, which on Windows is the Idle class, so they take the cores no other
# work needs.
#
# Env overrides:
#   SCL_LINT_NICE  niceness of the scripts (default: 19)
#
set -uo pipefail
here="$(dirname "$0")"
tools=(clang_format clang_tidy cppcheck doxygen doc_snippets)
SCL_LINT_NICE="${SCL_LINT_NICE:-19}"

reports=$(mktemp -d)
trap 'rm -rf "$reports"' EXIT

for tool in "${tools[@]}"; do
    {
        nice -n "$SCL_LINT_NICE" bash "${here}/${tool}.sh" > "$reports/$tool" 2>&1
        echo "$?" > "$reports/$tool.status"
    } &
done
wait

rc=0
for tool in "${tools[@]}"; do
    echo "=== ${tool} ==="
    cat "$reports/$tool"
    [ "$(cat "$reports/$tool.status" 2>/dev/null)" = 0 ] || rc=1
done

echo
if [ "$rc" -eq 0 ]; then
    echo "=== ALL CLEAN ==="
else
    echo "=== LINT FAILED ==="
fi
exit "$rc"
