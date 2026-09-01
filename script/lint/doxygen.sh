#!/usr/bin/env bash
#
# Doxygen lint — single source of truth for CI and local (IDE) runs.
# Run by the doxygen job in .gitlab-ci.yml and in .github/workflows/lint.yml.
#
# Fails on every Doxygen diagnostic. A block that reaches no entity, a @param
# naming a parameter that was renamed, a @ref to a symbol that moved: each one
# costs the reference a piece of what was written for it, and none of them fails
# anything else. The reference is published from this same Doxyfile, so a clean
# run here is what a reader gets there.
#
# The one line that is not a finding is Doxygen's own complaint about the output
# being switched off below; it is excluded by name.
#
# The Doxyfile is fed through stdin with every output format turned off, so the
# run stops after parsing: the diagnostics are the same as a full run's, it takes
# a fraction of the time, and nothing is written to the documentation directory.
#
# Env overrides:
#   DOXYFILE   Doxyfile to run (default: project/doxygen/Doxyfile)
#   DOXYGEN    doxygen executable (default: doxygen on PATH)
#
set -uo pipefail
cd "$(dirname "$0")/../.."

DOXYFILE="${DOXYFILE:-project/doxygen/Doxyfile}"
DOXYGEN="${DOXYGEN:-doxygen}"

[ -f "$DOXYFILE" ] || { echo "No Doxyfile ($DOXYFILE), skipping."; exit 0; }

command -v "$DOXYGEN" > /dev/null || { echo "No doxygen ($DOXYGEN) on PATH."; exit 1; }

LOG=$(mktemp)
trap 'rm -f "$LOG"' EXIT

{
    cat "$DOXYFILE"
    printf 'GENERATE_HTML=NO\nGENERATE_XML=NO\nGENERATE_MAN=NO\nGENERATE_LATEX=NO\n'
} | "$DOXYGEN" - > /dev/null 2> "$LOG"
STATUS=$?

if [ "$STATUS" -ne 0 ]; then
    cat "$LOG"
    echo
    echo "Doxygen exited with $STATUS."
    exit 1
fi

FINDINGS=$(grep -v 'No output formats selected!' "$LOG" | grep -c '' || true)

if [ "$FINDINGS" -ne 0 ]; then
    grep -v 'No output formats selected!' "$LOG"
    echo
    echo "Doxygen diagnostics: $FINDINGS"
    exit 1
fi

echo "No Doxygen diagnostics."
