#pragma once

#include "../ClangTidyCheck.h"
#include "dkaqmodule.h"

namespace clang {
namespace tidy {
namespace dkaq {

/// Checks that struct, enum, and union declarations have names.
///
/// For example, the following code will trigger the check:
/// \code
///   struct { int x; int y; } point; // No struct tag name
///   enum { RED, GREEN, BLUE } color; // No enum tag name
///   union { float f; int i; } value; // No union tag name
/// \endcode
class NamedDeclarationCheck : public DkaqClangTidyCheck {
public:
  NamedDeclarationCheck(StringRef Name, ClangTidyContext *Context)
      : DkaqClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace dkaq
} // namespace tidy
} // namespace clang
