#!/bin/sh
# Deploy versioned Doxygen docs for this module.
# Must be run from the module root directory.
#
# Usage: sh script/ci/deploy_versioned_docs.sh <pages_branch> [dryrun]
#   pages_branch — gh-pages (GitHub) or pages (GitLab)
#   dryrun       — skip push, keep output dir, print its path
#
# Behaviour:
#   - All version tags: built once, skipped if already deployed (immutable).
#   - main: always rebuilt, aliased "latest".
#   - dev: rebuilt if branch exists.
#   - Every HTML page gets an injected nav bar with a version selector.

set -e

PAGES_BRANCH="${1:-pages}"
DRY_RUN="${2:-}"
DOXYFILE="project/doxygen/Doxyfile"

# Derive display name from Doxyfile PROJECT_NAME (replace dot with space)
PROJECT_NAME=$(grep '^PROJECT_NAME\s*=' "${DOXYFILE}" | sed 's/.*=\s*//' | sed 's/\s*$//')
DISPLAY_NAME=$(echo "${PROJECT_NAME}" | tr '.' ' ')

# On Windows, doxygen is a native binary and needs Windows-style paths.
# cygpath -w converts /tmp/... → C:\...; on Linux it's a no-op passthrough.
native_path() {
  if command -v cygpath >/dev/null 2>&1; then cygpath -w "$1"; else echo "$1"; fi
}

BUILD_DIR=$(mktemp -d)
PAGES_WORK=$(mktemp -d)
ORIG_HEAD=$(git rev-parse HEAD)

cleanup() {
  rm -rf "${BUILD_DIR}"
  if [ -z "${DRY_RUN}" ]; then
    rm -rf "${PAGES_WORK}"
  fi
  git checkout "${ORIG_HEAD}" 2>/dev/null || true
}
trap cleanup EXIT

REMOTE_URL=$(git remote get-url origin)

git fetch --tags 2>/dev/null || true
TAGS=$(git tag -l 'v[0-9]*' 2>/dev/null | sort -V || true)

DEV_REF=""
if git rev-parse --verify dev >/dev/null 2>&1; then
  DEV_REF="dev"
elif git rev-parse --verify "origin/dev" >/dev/null 2>&1; then
  DEV_REF="origin/dev"
fi

echo "=== ${DISPLAY_NAME}, branch: ${PAGES_BRANCH} ==="
echo "    Tags: $(echo "$TAGS" | tr '\n' ' ')"

# ── Checkout pages branch ─────────────────────────────────────────────────────
git fetch origin "${PAGES_BRANCH}" 2>/dev/null || true
if git rev-parse --verify "origin/${PAGES_BRANCH}" >/dev/null 2>&1; then
  git clone --branch "${PAGES_BRANCH}" --single-branch "${REMOTE_URL}" "${PAGES_WORK}"
fi

# ── Read existing deployed versions ──────────────────────────────────────────
VERSIONS_FILE="${PAGES_WORK}/versions.json"
if [ -f "${VERSIONS_FILE}" ]; then
  EXISTING=$(python - "${VERSIONS_FILE}" << 'EOF'
import json, sys
data = json.load(open(sys.argv[1]))
for v in data: print(v['version'])
EOF
)
else
  EXISTING=""
fi
echo "    Existing: $(echo "$EXISTING" | tr '\n' ' ')"

# ── Pre-compute full versions list for nav bar ────────────────────────────────
VERSIONS_JSON=$(python - "${TAGS}" "${DEV_REF}" << 'EOF'
import sys
tags_raw, dev_ref = sys.argv[1], sys.argv[2]
tags = [t for t in tags_raw.split('\n') if t.strip()]
items = ['{"v":"main","label":"main (latest)"}']
for t in reversed(tags):
    items.append('{"v":"%s","label":"%s"}' % (t, t))
if dev_ref:
    items.append('{"v":"dev","label":"dev"}')
print('[' + ','.join(items) + ']')
EOF
)

# ── Helper: build Doxygen + inject nav bar ────────────────────────────────────
build_version() {
  VERSION="$1"
  OUT_DIR="${PAGES_WORK}/${VERSION}"

  rm -rf "${OUT_DIR}"
  mkdir -p "${OUT_DIR}"

  (cat "${DOXYFILE}";
   printf '\nGENERATE_HTML=YES\nGENERATE_XML=NO\nHTML_OUTPUT=.\nOUTPUT_DIRECTORY=%s\n' \
     "$(native_path "${OUT_DIR}")") | doxygen -

  python - "${OUT_DIR}" "${DISPLAY_NAME}" "${VERSION}" "${VERSIONS_JSON}" << 'EOF'
import os, sys
out_dir, display_name, version, versions_json = sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4]

nav = (
    '<div id="scl-nav" style="background:#1565c0;color:#fff;padding:8px 16px;'
    'font-family:sans-serif;display:flex;align-items:center;gap:12px;font-size:14px">'
    '<span style="font-weight:bold">' + display_name + '</span>'
    '<span style="opacity:.5">/</span>'
    '<select id="scl-ver" style="background:#0d47a1;color:#fff;'
    'border:1px solid rgba(255,255,255,.4);padding:3px 8px;cursor:pointer;font-size:13px">'
    '</select></div>\n'
    '<script>(function(){'
    'var V=' + versions_json + ';'
    'var cur="' + version + '";'
    'var s=document.getElementById("scl-ver");'
    'V.forEach(function(v){'
    'var o=document.createElement("option");'
    'o.value=v.v;o.text=v.label;'
    'if(v.v===cur)o.selected=true;'
    's.appendChild(o);});'
    's.onchange=function(){window.location="../"+s.value+"/";};'
    '})();</script>\n'
)

for root, dirs, files in os.walk(out_dir):
    for fname in files:
        if not fname.endswith('.html'):
            continue
        path = os.path.join(root, fname)
        try:
            content = open(path, encoding='utf-8', errors='ignore').read()
            idx = content.find('<body')
            if idx < 0:
                continue
            idx = content.find('>', idx) + 1
            open(path, 'w', encoding='utf-8').write(content[:idx] + nav + content[idx:])
        except Exception:
            pass
EOF
}

# ── Helper: update versions.json ─────────────────────────────────────────────
update_versions() {
  VERSION="$1"; ALIAS="$2"
  python - "${VERSIONS_FILE}" "${VERSION}" "${ALIAS}" << 'EOF'
import json, sys
f, ver, alias = sys.argv[1], sys.argv[2], sys.argv[3]
try:
    data = json.load(open(f))
except (FileNotFoundError, ValueError):
    data = []
data = [v for v in data if v['version'] != ver]
if alias:
    for v in data:
        if alias in v.get('aliases', []):
            v['aliases'].remove(alias)
entry = {'version': ver, 'title': ver, 'aliases': [alias] if alias else []}
if alias == 'latest':
    data.insert(0, entry)
elif ver == 'dev':
    data.append(entry)
else:
    idx = next((i for i, v in enumerate(data) if v['version'] == 'dev'), len(data))
    data.insert(idx, entry)
with open(f, 'w') as fp:
    json.dump(data, fp, indent=2)
EOF
}

# ── Helper: regenerate root index.html ───────────────────────────────────────
update_index() {
  python - "${VERSIONS_FILE}" "${PAGES_WORK}/index.html" "${DISPLAY_NAME}" << 'EOF'
import json, sys
vf, idx_file, display_name = sys.argv[1], sys.argv[2], sys.argv[3]
try:
    data = json.load(open(vf))
except (FileNotFoundError, ValueError):
    data = []
items = ''
for v in data:
    alias = ' <em>(%s)</em>' % ', '.join(v['aliases']) if v.get('aliases') else ''
    items += '<li><a href="%s/">%s</a>%s</li>\n' % (v['version'], v['title'], alias)
html = '''<!DOCTYPE html>
<html><head><meta charset="utf-8">
<meta http-equiv="refresh" content="0; url=main/">
<title>{n} - Versions</title>
<style>body{{font-family:sans-serif;max-width:600px;margin:2em auto}}</style>
</head><body><h1>{n}</h1><ul>{items}</ul>
</body></html>'''.format(n=display_name, items=items)
open(idx_file, 'w').write(html)
EOF
}

# ── Build version tags (skip already deployed) ────────────────────────────────
for TAG in $TAGS; do
  if echo "${EXISTING}" | grep -qxF "${TAG}"; then
    echo "  [skip] ${TAG}"
    continue
  fi
  echo "  [build] ${TAG}"
  git checkout "${TAG}"
  build_version "${TAG}"
  update_versions "${TAG}" ""
done

# ── Build main (always) ───────────────────────────────────────────────────────
echo "  [build] main -> latest"
git checkout main 2>/dev/null || git checkout "origin/main"
build_version "main"
update_versions "main" "latest"

# ── Build dev (if exists) ─────────────────────────────────────────────────────
if [ -n "$DEV_REF" ]; then
  echo "  [build] dev"
  git checkout "${DEV_REF}"
  build_version "dev"
  update_versions "dev" ""
fi

update_index

# ── Commit and push ───────────────────────────────────────────────────────────
if [ -n "${DRY_RUN}" ]; then
  echo "  [dryrun] output: ${PAGES_WORK}"
else
  git -C "${PAGES_WORK}" config user.email "$(git config user.email)"
  git -C "${PAGES_WORK}" config user.name "$(git config user.name)"
  git -C "${PAGES_WORK}" remote set-url origin "${REMOTE_URL}"
  git -C "${PAGES_WORK}" add -A
  if git -C "${PAGES_WORK}" diff --cached --quiet; then
    echo "  [no changes]"
  else
    git -C "${PAGES_WORK}" commit -m "docs: deploy versioned docs"
    git -C "${PAGES_WORK}" push origin "${PAGES_BRANCH}"
  fi
fi

echo "=== Done ==="
