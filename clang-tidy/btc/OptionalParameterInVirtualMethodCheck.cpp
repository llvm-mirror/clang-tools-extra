//===--- OptionalParameterInVirtualMethodCheck.cpp - clang-tidy------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "OptionalParameterInVirtualMethodCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace btc {

void OptionalParameterInVirtualMethodCheck::registerMatchers(
    MatchFinder *Finder) {
  Finder->addMatcher(
      parmVarDecl(hasDeclContext(cxxMethodDecl(isVirtual()))).bind("param"),
      this);
}

void OptionalParameterInVirtualMethodCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *MatchedDecl = Result.Nodes.getNodeAs<ParmVarDecl>("param");
  if (!MatchedDecl->hasDefaultArg() ||
      MatchedDecl
          ->hasUninstantiatedDefaultArg())  // Fot templates template<class...T>
                                            // void h(int i = 0, T... args);
    return;
  const auto *MatchedFunc =
      llvm::dyn_cast_or_null<const FunctionDecl>(MatchedDecl->getDeclContext());
  diag(MatchedDecl->getDefaultArg()->getExprLoc(),
       "parameter '%0' of virtual function '%1' is optional")
      << MatchedDecl->getName() << MatchedFunc->getName();
}

}  // namespace btc
}  // namespace tidy
}  // namespace clang
