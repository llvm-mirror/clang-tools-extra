//===--- SingletonCheck.h - clang-tidy---------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BTC_SINGLETONCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BTC_SINGLETONCHECK_H

#include "../ClangTidy.h"

namespace clang {
namespace tidy {
namespace btc {

/// This test searches for uses of Singleton pattern.
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/btc-singleton.html
class SingletonCheck : public ClangTidyCheck {
 public:
  SingletonCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

}  // namespace btc
}  // namespace tidy
}  // namespace clang

#endif  // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BTC_SINGLETONCHECK_H
