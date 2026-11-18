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

# The region of a source file between its two markers, for the diff of a block that does not
# match it.
extract_region() {
    awk -v marker="//! [$2]" '
        index($0, marker) { ++seen; next }
        seen == 1 { print }
    ' "$1"
}

# The first fenced code block that follows a given line, for the same diff.
extract_block() {
    awk -v start="$2" '
        NR <= start { next }
        /^```/ { if (fenced) exit; fenced = 1; next }
        fenced { print }
    ' "$1"
}

# One awk process reads every page and every source it names. It prints one record per
# finding, tab-separated, and the record "C <count>" last:
#   E <message>                     a marker that cannot be checked
#   M <page> <line> <path> <region> a block that differs from its source
#   W <page> <line> <path> <region> a block rewritten under --write
#   F <page>                        a page whose rewrite waits in <page>.snippet
check_pages() {
    awk -v write="$write" '
        # The region between the two markers of a source file, cached per file and region.
        # The state is "ok", "missing" for a file that cannot be read, or "incomplete" when
        # the file does not hold exactly the opening and the closing marker.
        function region(path, name,    key, marker, line, result, seen, count, text) {
            key = path SUBSEP name
            if (key in region_state)
                return region_state[key]
            marker = "//! [" name "]"
            seen = 0
            count = 0
            text = ""
            while ((result = (getline line < path)) > 0) {
                if (index(line, marker)) {
                    ++seen
                    continue
                }
                if (seen == 1) {
                    text = count == 0 ? line : text "\n" line
                    ++count
                }
            }
            close(path)
            region_text[key] = drop_trailing_newlines(text)
            if (result < 0)
                region_state[key] = "missing"
            else if (seen != 2)
                region_state[key] = "incomplete"
            else
                region_state[key] = "ok"
            return region_state[key]
        }

        function drop_trailing_newlines(text) {
            while (text ~ /\n$/)
                sub(/\n$/, "", text)
            return text
        }

        function check_page(page,    i, rest, words, count, path, name, state, opening, closing,
                            k, text, rewrites) {
            rewrites = 0
            split("", replaced)
            for (i = 1; i <= lines; ++i) {
                if (!index(line_of[i], "<!-- snippet:"))
                    continue
                rest = substr(line_of[i], index(line_of[i], "<!-- snippet:") + 13)
                if (index(rest, "-->"))
                    rest = substr(rest, 1, index(rest, "-->") - 1)
                gsub(/^[ \t]+|[ \t]+$/, "", rest)
                count = split(rest, words, /[ \t]+/)
                path = count >= 1 ? words[1] : ""
                name = count >= 2 ? words[2] : "quick_start"

                state = path == "" ? "missing" : region(path, name)
                if (state == "missing") {
                    printf "E\t%s:%d: snippet source does not exist: %s\n", page, i, path
                    continue
                }
                ++checked
                if (state == "incomplete") {
                    printf "E\t%s:%d: %s holds no complete [%s] region\n", page, i, path, name
                    continue
                }

                opening = 0
                closing = 0
                for (k = i + 1; k <= lines; ++k)
                    if (line_of[k] ~ /^```/) {
                        if (opening) {
                            closing = k
                            break
                        }
                        opening = k
                    }
                if (!closing) {
                    printf "E\t%s:%d: the snippet marker is followed by no complete code block\n", page, i
                    continue
                }

                text = ""
                for (k = opening + 1; k < closing; ++k)
                    text = k == opening + 1 ? line_of[k] : text "\n" line_of[k]
                if (drop_trailing_newlines(text) "" == region_text[path SUBSEP name] "")
                    continue

                if (write) {
                    replaced[opening] = closing
                    replacement[opening] = region_text[path SUBSEP name]
                    ++rewrites
                    printf "W\t%s\t%d\t%s\t%s\n", page, i, path, name
                } else
                    printf "M\t%s\t%d\t%s\t%s\n", page, i, path, name
            }

            if (!rewrites)
                return
            for (i = 1; i <= lines; ++i) {
                print line_of[i] > (page ".snippet")
                if (i in replaced) {
                    print replacement[i] > (page ".snippet")
                    i = replaced[i] - 1
                }
            }
            close(page ".snippet")
            printf "F\t%s\n", page
        }

        FNR == 1 && NR > 1 { check_page(page) }
        FNR == 1 { page = FILENAME; lines = 0 }
        { line_of[++lines] = $0 }
        END {
            if (NR > 0)
                check_page(page)
            printf "C\t%d\n", checked
        }
    ' "$@"
}

dirs=()
for dir in $SCL_DOC_DIRS; do
    [ -d "$dir" ] && dirs+=("$dir")
done

[ "${#dirs[@]}" -ne 0 ] || { echo "No documentation ($SCL_DOC_DIRS), skipping."; exit 0; }

pages=()
while IFS= read -r -d '' page; do
    pages+=("$page")
done < <(find "${dirs[@]}" -name '*.md' -not -path './.git/*' -not -path './doc/doxygen/*' -print0 |
    sort -z)

status=0
finished=0
checked=0
written=0

while IFS=$'\t' read -r kind page line source_path region; do
    case "$kind" in
    E)
        echo "$page"
        status=1
        ;;
    M)
        diff -u \
            --label "$source_path [$region]" <(extract_region "$source_path" "$region") \
            --label "$page:$line" <(extract_block "$page" "$line")
        echo "$page:$line: block does not match $source_path [$region]"
        status=1
        ;;
    W)
        echo "$page:$line: filled in from $source_path [$region]"
        written=$((written + 1))
        ;;
    F)
        mv -- "$page.snippet" "$page"
        ;;
    C)
        checked="$page"
        finished=1
        ;;
    esac
done < <([ "${#pages[@]}" -eq 0 ] || check_pages "${pages[@]}")

if [ "${#pages[@]}" -ne 0 ] && [ "$finished" -eq 0 ]; then
    echo
    echo "Documentation snippet check did not finish."
    exit 1
fi

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
