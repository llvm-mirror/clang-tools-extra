set(LLVM_LINK_COMPONENTS support)

add_clang_library(clangTidyBtcModule
  BtcTidyModule.cpp
  UnmanagedDerivedFromSharableCheck.cpp
  CallingSfehlerWithStopOnErrorCheck.cpp
  GlobalVariablesCheck.cpp
  MultipleParametersOfSameTypeCheck.cpp
  NoAssignmentToReferenceParameterCheck.cpp
  OptionalParameterInVirtualMethodCheck.cpp
  SingletonCheck.cpp
  CABExceptionCheck.cpp

  LINK_LIBS
  clangAST
  clangASTMatchers
  clangBasic
  clangLex
  clangTidy
  clangTidyUtils
  )
