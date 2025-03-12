#pragma once

#include "../ClangTidyCheck.h"
#include "dkaqmodule.h"

namespace clang {
namespace tidy {
namespace dkaq {

/// Checks that all function parameters have explicit type declarations.
///
/// For example, the following code will trigger the check:
/// \code
///   int func(height); // Warning: parameter 'height' has no type specifier
/// \endcode
///
/// The following code will not trigger the check:
/// \code
///   int func(int height); // OK: parameter has explicit type
/// \endcode
class ExplicitParameterTypeCheck : public DkaqClangTidyCheck {
public:
  ExplicitParameterTypeCheck(StringRef Name, ClangTidyContext *Context)
      : DkaqClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace dkaq
} // namespace tidy
} // namespace clang
