//===--- UnmanagedDerivedFromSharableCheck.cpp - clang-tidy----------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "UnmanagedDerivedFromSharableCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace btc {

const auto IsDerivedFromSharable =
    cxxRecordDecl(isDerivedFrom("Sharable")).bind("Derived");
const auto HasTypeDerivedFromSharable = anyOf(
    hasType(IsDerivedFromSharable), hasType(references(IsDerivedFromSharable)));

void UnmanagedDerivedFromSharableCheck::registerMatchers(
    MatchFinder *Finder) {
  Finder->addMatcher(fieldDecl(HasTypeDerivedFromSharable).bind("fields"),
                     this);
  Finder->addMatcher(
      varDecl(allOf(hasLocalStorage(), HasTypeDerivedFromSharable))
          .bind("localvars"),
      this);
}

void UnmanagedDerivedFromSharableCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *MatchedFieldDecl = Result.Nodes.getNodeAs<FieldDecl>("fields");
  if (MatchedFieldDecl)
    diag(MatchedFieldDecl->getLocation(),
         "Field %0 has type derived from Sharable")
        << MatchedFieldDecl;

  const auto *MatchedLocalVarDecl =
      Result.Nodes.getNodeAs<VarDecl>("localvars");
  if (MatchedLocalVarDecl && MatchedLocalVarDecl->isLocalVarDecl())
    diag(MatchedLocalVarDecl->getLocation(),
         "Local variable %0 has type derived from Sharable")
        << MatchedLocalVarDecl;
}

}  // namespace btc
}  // namespace tidy
}  // namespace clang
