#!/usr/bin/env bash
#
# Run the whole lint stage (clang-format, clang-tidy, cppcheck, doxygen, doc snippets) the
# same way CI does. Each tool has its own script; this umbrella runs all five and aggregates
# the result. Intended for local (IDE) use — CI runs the same scripts, grouped into jobs
# by the image each one needs.
#
set -uo pipefail
here="$(dirname "$0")"

rc=0
for tool in clang_format clang_tidy cppcheck doxygen doc_snippets; do
    echo "=== ${tool} ==="
    bash "${here}/${tool}.sh" || rc=1
done

echo
if [ "$rc" -eq 0 ]; then
    echo "=== ALL CLEAN ==="
else
    echo "=== LINT FAILED ==="
fi
exit "$rc"
