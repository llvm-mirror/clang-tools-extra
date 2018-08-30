TRAVIS_BUILD_DIR="$1/"
TOP_SRC_DIR="$2"
BUILD_DIR="$3"
ACTUAL_SRC_DIR="${TOP_SRC_DIR}tools/clang/tools/extra"

if [[ -d $HOME/llvm/tools/clang ]]; then
  cd "${TOP_SRC_DIR}" && git pull 
  cd "${TOP_SRC_DIR}tools/clang/" && git pull
else
  git clone --depth=2 https://github.com/llvm-mirror/llvm.git "$HOME/tmp/" -b release_70 
  rsync -az "$HOME/tmp/" "${TOP_SRC_DIR}"
  git clone --depth=2 https://github.com/llvm-mirror/clang.git "${TOP_SRC_DIR}tools/clang/" -b release_70       
  mkdir -p "${ACTUAL_SRC_DIR}"
  mkdir -p "${BUILD_DIR}"
fi
rsync -az "$TRAVIS_BUILD_DIR" "${ACTUAL_SRC_DIR}"
cd "${BUILD_DIR}"
if [[ -f touch_mtimes.txt ]]; then 
  while read mtime fn; do touch -c -m -d "@$mtime" $fn; done < touch_mtimes.txt
  cut -f 1 -d ' ' touch_mtimes.txt | sort | read OLDEST_MTIME
  export OLDEST_MTIME=$((OLDEST_MTIME-1))
  find "${ACTUAL_SRC_DIR}" -type f -exec touch -c -m -d "@$OLDEST_MTIME" {} \
  cd "${ACTUAL_SRC_DIR}"
  PREVIOUS_GIT_COMMIT="HEAD~1"
  if [[ -f previous_git_commit.txt ]]; then read PREVIOUS_GIT_COMMIT < previous_git_commit.txt; fi;
  export PREVIOUS_GIT_COMMIT
  changed_files=`git diff --name-only $PREVIOUS_GIT_COMMIT HEAD`
  echo "Previously cached Travis build based on git commit ${PREVIOUS_GIT_COMMIT}."
  echo "... changed files since then:"
  echo $changed_files
  touch `echo $changed_files`
else
  cmake -G "Unix Makefiles"  -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local -DLLVM_TARGETS_TO_BUILD="X86" "${TOP_SRC_DIR}"
fi

