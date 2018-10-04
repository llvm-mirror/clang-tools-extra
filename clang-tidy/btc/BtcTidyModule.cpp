//===--- BtcTidyModule.cpp - clang-tidy---------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "../ClangTidy.h"
#include "../ClangTidyModule.h"
#include "../ClangTidyModuleRegistry.h"
#include "UnmanagedDerivedFromSharableCheck.h"
#include "CallingSfehlerWithStopOnErrorCheck.h"
#include "GlobalVariablesCheck.h"
#include "MultipleParametersOfSameTypeCheck.h"
#include "NoAssignmentToReferenceParameterCheck.h"
#include "OptionalParameterInVirtualMethodCheck.h"
#include "SingletonCheck.h"
#include "CABExceptionCheck.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace btc {

/// This module is for Btc-specific checks.
class BtcModule : public ClangTidyModule {
public:
  void addCheckFactories(ClangTidyCheckFactories &CheckFactories) override {
    CheckFactories.registerCheck<UnmanagedDerivedFromSharableCheck>(
        "btc-unmanaged-derived-from-sharable");
    CheckFactories.registerCheck<CallingSfehlerWithStopOnErrorCheck>(
        "btc-calling-SFehler-with-stop-on-error");
    CheckFactories.registerCheck<GlobalVariablesCheck>(
        "btc-global-variables");
    CheckFactories.registerCheck<MultipleParametersOfSameTypeCheck>(
        "btc-multiple-parameters-of-same-type");
    CheckFactories.registerCheck<NoAssignmentToReferenceParameterCheck>(
        "btc-no-assignment-to-reference-parameter");
    CheckFactories.registerCheck<OptionalParameterInVirtualMethodCheck>(
        "btc-optional-parameter-in-virtual-method");
    CheckFactories.registerCheck<SingletonCheck>(
        "btc-singleton");
    CheckFactories.registerCheck<btc::CABExceptionCheck>(
        "btc-use-of-cab-exception");
  }
};

// Register the BtcTidyModule using this statically initialized variable.
static ClangTidyModuleRegistry::Add<BtcModule>
    X("btc-module", "Adds Btc checks.");
} // namespace btc

// This anchor is used to force the linker to link in the generated object file
// and thus register the BtcModule.
volatile int BtcModuleAnchorSource = 0;

} // namespace tidy
} // namespace clang
