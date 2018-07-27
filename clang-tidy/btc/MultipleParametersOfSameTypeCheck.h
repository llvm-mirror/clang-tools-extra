//===--- MultipleParametersOfSameTypeCheck.h - clang-tidy--------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BTC_MULTIPLEPARAMETERSOFSAMETYPECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BTC_MULTIPLEPARAMETERSOFSAMETYPECHECK_H

#include "../ClangTidy.h"

namespace clang {
namespace tidy {
namespace btc {

/// This check targets function declarations with multiple parameters of the
/// same type
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/btc-multiple-parameters-of-same-type.html
class MultipleParametersOfSameTypeCheck : public ClangTidyCheck {
 public:
  MultipleParametersOfSameTypeCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

}  // namespace btc
}  // namespace tidy
}  // namespace clang

#endif  // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BTC_MULTIPLEPARAMETERSOFSAMETYPECHECK_H
