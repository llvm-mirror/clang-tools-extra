//===--- GlobalVariablesCheck.cpp - clang-tidy-----------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "GlobalVariablesCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace btc {

void GlobalVariablesCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(varDecl(hasGlobalStorage()).bind("Var"), this);
}

void GlobalVariablesCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedVarDecl = Result.Nodes.getNodeAs<VarDecl>("Var");
  if (MatchedVarDecl->getAccess() == AS_none ||
      MatchedVarDecl->getAccess() == AS_public) {
    // It is not part of a class or is a public static field.
    if (!MatchedVarDecl->isStaticLocal()) {
      // and it is not a static local variable.
      // At this point I hope it must be a global variable.
      diag(MatchedVarDecl->getLocStart(), "Global variable detected.");
    }
  }
}

}  // namespace btc
}  // namespace tidy
}  // namespace clang
