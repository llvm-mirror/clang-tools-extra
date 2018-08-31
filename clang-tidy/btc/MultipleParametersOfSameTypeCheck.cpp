//===--- MultipleParametersOfSameTypeCheck.cpp - clang-tidy----------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "MultipleParametersOfSameTypeCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace btc {

void MultipleParametersOfSameTypeCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(functionDecl().bind("func"), this);
}

void MultipleParametersOfSameTypeCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *MatchedDecl = Result.Nodes.getNodeAs<FunctionDecl>("func");
  std::unordered_map<std::string, std::set<const ParmVarDecl *> > typeMap;
  for (unsigned int i = 0; i < MatchedDecl->getNumParams(); ++i) {
    const auto *paramDecl = MatchedDecl->getParamDecl(i);
    typeMap[paramDecl->getType().getCanonicalType().getAsString()].insert(
        paramDecl);
  }
  bool firstDiag = true;

  for (auto &&entry : typeMap) {
    if (entry.second.size() > 1) {
      if (firstDiag) {
        diag(MatchedDecl->getLocation(),
             "function '%0' has multiple parameters of same type")
            << MatchedDecl->getNameAsString();
        firstDiag = false;
      }
      for (auto &&param : entry.second) {
        if (param->getDeclName().isEmpty())
          continue; /** \todo output index in this case! */

        diag(param->getLocation(),
             "parameter '%0' with type '%1' (total number of parameters with "
             "this type: %2)")
            << param->getNameAsString() << param->getType()
            << (unsigned int)entry.second.size();
      }
    }
  }
}
}  // namespace btc
}  // namespace tidy
}  // namespace clang
