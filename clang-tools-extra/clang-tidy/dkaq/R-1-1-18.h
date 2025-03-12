#pragma once

#include "../ClangTidyCheck.h"
#include "dkaqmodule.h"

namespace clang {
namespace tidy {
namespace dkaq {

/// Checks that all array definitions have explicit size bounds.
///
/// For example, the following code will trigger the check:
/// \code
///   int a[] = {0, 1, 2}; // Warning: array should have explicit size
/// \endcode
///
/// The following code will not trigger the check:
/// \code
///   int a[3] = {0, 1, 2}; // OK: explicit size
/// \endcode
class ExplicitArrayBoundsCheck : public DkaqClangTidyCheck {
public:
  ExplicitArrayBoundsCheck(StringRef Name, ClangTidyContext *Context)
      : DkaqClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace dkaq
} // namespace tidy
} // namespace clang
