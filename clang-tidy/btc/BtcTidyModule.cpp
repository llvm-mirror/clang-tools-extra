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
