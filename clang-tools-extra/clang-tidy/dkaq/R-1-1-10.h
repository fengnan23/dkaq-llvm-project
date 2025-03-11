#pragma once

#include "../ClangTidyCheck.h"

 
namespace clang {
namespace tidy {
namespace dkaq {
 


class BitFieldWidthCheck : public ClangTidyCheck {
public:
  BitFieldWidthCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};



}
} // namespace tidy 
} // namespace clang 