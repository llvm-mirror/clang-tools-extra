//===--- NoAssignmentToReferenceParameterCheck.h - clang-tidy----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BTC_NOASSIGNMENTTOREFERENCEPARAMETERCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BTC_NOASSIGNMENTTOREFERENCEPARAMETERCHECK_H

#include "../ClangTidy.h"

namespace clang {
namespace tidy {
namespace btc {

/// For working example see btc-no-assignment-to-reference-parameter.cpp in test
/// folder.
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/btc-no-assignment-to-reference-parameter.html
class NoAssignmentToReferenceParameterCheck : public ClangTidyCheck {
 public:
  NoAssignmentToReferenceParameterCheck(StringRef Name,
                                        ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

}  // namespace btc
}  // namespace tidy
}  // namespace clang

#endif  // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BTC_NOASSIGNMENTTOREFERENCEPARAMETERCHECK_H
