#pragma once

#include "../ClangTidyCheck.h"

namespace clang {
namespace tidy {
namespace dkaq {

/// Checks that extern variables are not declared inside function bodies.
///
/// For example, the following code will trigger the check:
/// \code
///   void func() {
///     extern int global_var; // Warning: extern declaration inside function
///   }
/// \endcode
class NoExternInFunctionCheck : public ClangTidyCheck {
public:
  NoExternInFunctionCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace dkaq
} // namespace tidy
} // namespace clang
