#!/bin/sh

# based on https://blog.esciencecenter.nl/travis-caching-and-incremental-builds-6518b89ee889

set -x -e

LLVM_CLANG_TOOLS_EXTRA_SRC_DIR=$1
LLVM_SRC_ROOT=$2

BUILD_DIR=$(pwd)
echo "Restored commit hash: $(cat previous_git_commit.txt)"

if [ "$3" == "ninja" ] ; then
    echo "Skipping restoration of build file time stamps"
    
    OLDEST_MTIME=$(find . -type f -printf "%.10T@\n" | sort | head -1)
else
    echo "Restored mtimes of $(cat touch_mtimes.txt | wc -l) files in cmake build dir"

    set +x
    MISSING_FILES=0
    while read mtime fn; do
        if [[ -f $fn ]] ; then
            touch -c -m -d "@$mtime" $fn
        else
            MISSING_FILES=1
            echo "WARNING: File ${fn} not found, cannot restore its mtime"        
        fi
    done < touch_mtimes.txt
    set -x

    if [ "${MISSING_FILES}" == "1" ] ; then
        find
    fi

    OLDEST_MTIME=$(cut -f 1 -d ' ' touch_mtimes.txt | sort | head -1)
fi

# In any case, rebuilding  with ninja works slightly differently than with make, 
# since the absolute mtimes are compared, not just the order. The mtimes of the 
# build files themselves must not be changed. So if we cannot change those, the 
# only other option is to make all the cloned source files older than the oldest 
# build file:

if [ "$OLDEST_MTIME" == "" ] ; then
    echo "ERROR: OLDEST_MTIME is not set"
    exit 1
fi

export OLDEST_MTIME=$((OLDEST_MTIME-1))
touch -c -m -d "@$OLDEST_MTIME" $(find "${LLVM_SRC_ROOT}" -type f)

cd "${LLVM_CLANG_TOOLS_EXTRA_SRC_DIR}"

PREVIOUS_GIT_COMMIT="HEAD~1"
if [[ -f ${BUILD_DIR}/previous_git_commit.txt ]]; then 
    read PREVIOUS_GIT_COMMIT < ${BUILD_DIR}/previous_git_commit.txt
else
    echo "WARNING: previous_git_commit.txt not found"
fi
export PREVIOUS_GIT_COMMIT
if git log -n1 $PREVIOUS_GIT_COMMIT >/dev/null ; then
    changed_files=$(git diff --name-only $PREVIOUS_GIT_COMMIT HEAD)
    echo "Previously cached Travis build based on git commit ${PREVIOUS_GIT_COMMIT}."
    echo "... changed files since then:"
    echo $changed_files
    touch $(echo $changed_files)
else
    echo "WARNING: previous commit ${PREVIOUS_GIT_COMMIT} not found in local git, maybe it is too old or it has been removed from the history"
fi

