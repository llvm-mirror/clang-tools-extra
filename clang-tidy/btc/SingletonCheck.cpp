//===--- SingletonCheck.cpp - clang-tidy-----------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "SingletonCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace btc {

void SingletonCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      cxxRecordDecl(isDerivedFrom("Singleton")).bind("Singleton"), this);
}

void SingletonCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedStmt = Result.Nodes.getNodeAs<CXXRecordDecl>("Singleton");
  diag(MatchedStmt->getLocStart(), "Use of singleton detected.");
}

}  // namespace btc
}  // namespace tidy
}  // namespace clang
