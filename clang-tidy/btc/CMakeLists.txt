set(LLVM_LINK_COMPONENTS support)

add_clang_library(clangTidyBtcModule
  BtcTidyModule.cpp
  UnmanagedDerivedFromSharableCheck.cpp

  LINK_LIBS
  clangAST
  clangASTMatchers
  clangBasic
  clangLex
  clangTidy
  clangTidyUtils
  )
