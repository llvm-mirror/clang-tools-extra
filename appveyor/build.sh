#!/bin/sh

set -x

cd ${LLVM_CMAKE_BUILD_ROOT}
if [[ -f "${LLVM_CMAKE_BUILD_ROOT}/touch_mtimes.txt" ]] ; then
    ${MOVED_BUILD_FOLDER}/travis-ci/restore-mtimes.sh $(cygpath -au ${LLVM_ROOT}/llvm/tools/clang/tools/extra) $(cygpath -au ${LLVM_ROOT}/llvm) ninja
fi
if ! timeout 2700 ${NINJA_DIR}/ninja -d explain clang-tidy ; then 
    echo "ERROR: build failed ($?)"
fi
${MOVED_BUILD_FOLDER}/travis-ci/store-mtimes.sh $(cygpath -au ${MOVED_BUILD_FOLDER})
# - ninja check-all # I wouldn't sweat a few failures here. This step is optional
# - ninja install
