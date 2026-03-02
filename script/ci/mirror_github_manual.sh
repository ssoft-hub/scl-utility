#!/bin/sh
# Manual mirror trigger for scl-utility → GitHub.
# Usage: sh script/ci/mirror_github_manual.sh

set -e

DIR="$(cd "$(dirname "$0")" && pwd)"

GITHUB_MIRROR_URL="git@github.com:ssoft-hub/scl-utility.git" \
sh "$DIR/mirror_github.sh"
