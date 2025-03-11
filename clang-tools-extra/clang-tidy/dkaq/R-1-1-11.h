//===--- ExplicitBitFieldSignCheck.h - clang-tidy --------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#pragma once

#include "../ClangTidyCheck.h"

namespace clang {
namespace tidy {
namespace dkaq {

/// Checks that integer bit-fields in classes, structs, and unions have an
/// explicit sign specification (either 'signed' or 'unsigned').
///
/// For example, the following code will trigger the check:
/// \code
///   struct S {
///     int x : 5;  // Warning: sign not explicitly specified
///   };
/// \endcode
///
/// The following code will not trigger the check:
/// \code
///   struct S {
///     signed int x : 5;    // OK: explicitly signed
///     unsigned int y : 5;  // OK: explicitly unsigned
///   };
/// \endcode
class ExplicitBitFieldSignCheck : public ClangTidyCheck {
public:
  ExplicitBitFieldSignCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace dkaq
} // namespace tidy
} // namespace clang

