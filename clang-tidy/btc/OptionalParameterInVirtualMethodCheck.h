//===--- OptionalParameterInVirtualMethodCheck.h - clang-tidy----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BTC_OPTIONALPARAMETERINVIRTUALMETHODCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BTC_OPTIONALPARAMETERINVIRTUALMETHODCHECK_H

#include "../ClangTidy.h"

namespace clang {
namespace tidy {
namespace btc {

/// Searches for default parameters in virtual functions.
/// For example of use see btc-optional-parameter-in-virtual-method.cpp in test
/// folder. For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/btc-optional-parameter-in-virtual-method.html
class OptionalParameterInVirtualMethodCheck : public ClangTidyCheck {
 public:
  OptionalParameterInVirtualMethodCheck(StringRef Name,
                                        ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

}  // namespace btc
}  // namespace tidy
}  // namespace clang

#endif  // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BTC_OPTIONALPARAMETERINVIRTUALMETHODCHECK_H
