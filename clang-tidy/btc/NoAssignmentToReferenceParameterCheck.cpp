//===--- NoAssignmentToReferenceParameterCheck.cpp - clang-tidy------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "NoAssignmentToReferenceParameterCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace btc {

void NoAssignmentToReferenceParameterCheck::registerMatchers(
    MatchFinder *Finder) {
  Finder->addMatcher(binaryOperator(hasOperatorName("="),
                                    hasLHS(declRefExpr().bind("declref"))),
                     this);
}
void NoAssignmentToReferenceParameterCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *MatchedDeclRef = Result.Nodes.getNodeAs<DeclRefExpr>("declref");
  const auto *decl = llvm::dyn_cast_or_null<const clang::ParmVarDecl>(
      MatchedDeclRef->getDecl());
  if (decl && decl->getType().getTypePtr()->isReferenceType()) {
    diag(MatchedDeclRef->getLocation(),
         "reference parameter %0 must not be assigned to parameter declared "
         "above with type %1")
        << decl->getName() << decl->getType();
  }
}
}  // namespace btc
}  // namespace tidy
}  // namespace clang
