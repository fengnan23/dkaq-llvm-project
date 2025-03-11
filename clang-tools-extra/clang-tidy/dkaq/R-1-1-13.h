#pragma once

#include "../ClangTidyCheck.h"

namespace clang {
namespace tidy {
namespace dkaq {

/// Checks that all function parameters in function declarations have names.
///
/// For example, the following code will trigger the check:
/// \code
///   // Warning: parameter #1 not named
///   int add(int, int b);
/// \endcode
class NamedParametersCheck : public ClangTidyCheck {
public:
  NamedParametersCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace dkaq
} // namespace tidy
} // namespace clang
