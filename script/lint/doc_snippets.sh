#!/usr/bin/env bash
#
# Documentation snippet lint — single source of truth for CI and local (IDE) runs.
# Run by the doxygen job in .gitlab-ci.yml and the doc-snippets job in
# .github/workflows/lint.yml.
#
# A Markdown code block that copies a compiled program drifts from it silently: the
# program keeps building, the page keeps rendering, and only a reader finds out. A block
# introduced by
#
#     <!-- snippet: example/quick_start/meta/meta.cpp -->
#
# must therefore repeat, character for character, the region of that file between the two
# `//! [quick_start]` markers. Naming another region is `<!-- snippet: <path> <region> -->`.
# The same markers are what Doxygen's @snippet reads, so the reference pulls the code in
# rather than copying it. Markdown has no include mechanism on any forge, so the pages
# carry copies and this script keeps them equal.
#
# `--write` fills the blocks in from their sources instead of comparing them. It is for
# local work — after editing an example, one command replaces the same edit repeated in
# every page. CI calls the script without it: a pipeline that wrote back would race the
# author, and could not push from a fork at all.
#
# A missing delimiter is reported rather than worked around: an unterminated region or
# fence would otherwise extend to the end of the file, and two extractions that both come
# out empty would compare equal and pass. Such a page is left untouched in both modes.
#
# Env overrides:
#   SCL_DOC_DIRS  directories to scan for Markdown, space-separated (default: . )
#
set -uo pipefail
cd "$(dirname "$0")/../.."

SCL_DOC_DIRS="${SCL_DOC_DIRS:-.}"

write=0
case "${1:-}" in
--write) write=1 ;;
"") ;;
*) echo "usage: $(basename "$0") [--write]" >&2; exit 2 ;;
esac

# The region of a source file between its two markers. Exits non-zero when the file does
# not hold exactly the opening and the closing one.
extract_region() {
    awk -v marker="//! [$2]" '
        index($0, marker) { ++seen; next }
        seen == 1 { print }
        END { if (seen != 2) exit 1 }
    ' "$1"
}

# The first fenced code block that follows a given line. Exits non-zero when that block is
# never opened or never closed.
extract_block() {
    awk -v start="$2" '
        NR <= start { next }
        /^```/ { if (fenced) { closed = 1; exit } fenced = 1; next }
        fenced { print }
        END { if (!closed) exit 1 }
    ' "$1"
}

# The page with the block that follows a given line replaced by the given text. The text
# travels through the environment rather than `awk -v`, which would read `\n` in the code
# as an escape and put a line break where the program has a character literal.
replace_block() {
    replacement="$3" awk -v start="$2" '
        NR <= start { print; next }
        !fenced { print; if (/^```/) { fenced = 1; printf "%s", ENVIRON["replacement"] } next }
        !closed { if (/^```/) { closed = 1; print } next }
        { print }
    ' "$1"
}

status=0
checked=0
written=0

dirs=()
for dir in $SCL_DOC_DIRS; do
    [ -d "$dir" ] && dirs+=("$dir")
done

[ "${#dirs[@]}" -ne 0 ] || { echo "No documentation ($SCL_DOC_DIRS), skipping."; exit 0; }

while IFS= read -r page; do
    # A rewrite shifts the line numbers below it, so the markers of one page are handled
    # from the bottom up and every one of them keeps the position grep reported.
    while IFS= read -r hit; do
        line="${hit%%:*}"
        rest="${hit#*<!-- snippet:}"
        rest="${rest%%-->*}"
        # shellcheck disable=SC2086 # deliberate word splitting: "<path> [region]"
        set -- $rest
        source_path="${1:-}"
        region="${2:-quick_start}"

        if [ ! -f "$source_path" ]; then
            echo "$page:$line: snippet source does not exist: $source_path"
            status=1
            continue
        fi

        checked=$((checked + 1))

        if ! source_text=$(extract_region "$source_path" "$region"); then
            echo "$page:$line: $source_path holds no complete [$region] region"
            status=1
            continue
        fi

        if ! block_text=$(extract_block "$page" "$line"); then
            echo "$page:$line: the snippet marker is followed by no complete code block"
            status=1
            continue
        fi

        [ "$source_text" = "$block_text" ] && continue

        if [ "$write" -eq 0 ]; then
            diff -u \
                --label "$source_path [$region]" <(printf '%s\n' "$source_text") \
                --label "$page:$line" <(printf '%s\n' "$block_text")
            echo "$page:$line: block does not match $source_path [$region]"
            status=1
            continue
        fi

        replace_block "$page" "$line" "$source_text
" > "$page.snippet" && mv -- "$page.snippet" "$page"
        echo "$page:$line: filled in from $source_path [$region]"
        written=$((written + 1))
    done < <(grep -n -- '<!-- snippet:' "$page" | sort -rn -t: -k1,1 || true)
done < <(find "${dirs[@]}" -name '*.md' -not -path './.git/*' -not -path './doc/doxygen/*')

if [ "$status" -ne 0 ]; then
    echo
    echo "Documentation snippets out of date."
    exit 1
fi

if [ "$write" -eq 1 ]; then
    echo "Documentation snippets written from their sources ($written of $checked changed)."
else
    echo "Documentation snippets match their sources ($checked checked)."
fi
