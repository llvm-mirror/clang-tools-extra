//===--- CallingSfehlerWithStopOnErrorCheck.cpp - clang-tidy---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "CallingSfehlerWithStopOnErrorCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace btc {

void CallingSfehlerWithStopOnErrorCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      callExpr(callee(functionDecl(hasName("SFehler"), parameterCountIs(6))))
          .bind("SFehler"),
      this);
}

void CallingSfehlerWithStopOnErrorCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedCallExpr = Result.Nodes.getNodeAs<CallExpr>("SFehler");
  const Expr *errorResult = MatchedCallExpr->getArg(5);
  llvm::APSInt errorResultValue;
  if (errorResult->EvaluateAsInt(errorResultValue, *Result.Context))
   if (errorResultValue == 1)
      diag(MatchedCallExpr->getLocStart(),
           "SFehler function called with STOPONERROR parameter.");
}

}  // namespace btc
}  // namespace tidy
}  // namespace clang
