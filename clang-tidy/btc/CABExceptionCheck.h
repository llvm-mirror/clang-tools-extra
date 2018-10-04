#pragma once
//===--- CABExceptionCheck.h - clangTidyBtcModule----*- C++ -*-===//
//    This Check is intended to detect the places in code
//    where BTC::Commons::Core::Exception  are
//    used.
//===-------------------------------------------------------===//

#include "../ClangTidy.h"
//#include "DeclCXX.h"

namespace clang {
namespace tidy {
namespace btc {

class CABExceptionCheck : public ClangTidyCheck {

  const std::string CABExceptionAsPointerParamName = "CABExceptionAsPointer";

public:
  CABExceptionCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context),
        mExceptionsAsPointers(
            Options.get(CABExceptionAsPointerParamName, false)) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

  void storeOptions(ClangTidyOptions::OptionMap &Options) override;

private:
  const bool mExceptionsAsPointers;
};

} // namespace btc
} // namespace tidy
} // namespace clang
