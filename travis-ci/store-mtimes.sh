#!/bin/sh

set -x -e

ACTUAL_SRC_DIR=$1

BUILD_DIR=$(pwd)
find . -type f -printf "%.10T@ %p\n" > touch_mtimes.txt
cd "${ACTUAL_SRC_DIR}" && git rev-parse HEAD > ${BUILD_DIR}/previous_git_commit.txt
echo "Stored commit hash: $(cat ${BUILD_DIR}/previous_git_commit.txt)"
echo "Stored mtimes of $(wc -l ${BUILD_DIR}/touch_mtimes.txt) files"
