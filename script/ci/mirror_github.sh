#!/bin/sh
# Push an exact mirror of all branches and tags to GitHub.
#
# Required variables:
#   GITHUB_MIRROR_URL  - git@github.com:org/repo.git  (manual, SSH)
#                      - https://github.com/org/repo.git  (CI, HTTPS)
#   GITHUB_TOKEN       - GitHub PAT, required only for HTTPS

set -e

if [ -n "$GITHUB_TOKEN" ]; then
    GITHUB_AUTH_URL="https://oauth2:${GITHUB_TOKEN}@${GITHUB_MIRROR_URL#https://}"
else
    GITHUB_AUTH_URL="$GITHUB_MIRROR_URL"
fi

git clone --mirror "$CI_REPOSITORY_URL" _mirror.git
cd _mirror.git
git remote add github "$GITHUB_AUTH_URL"
git push github --mirror
