#pragma once

#include "../ClangTidyCheck.h"
#include "dkaqmodule.h"
 
namespace clang {
namespace tidy {
namespace dkaq {
 


class BitFieldWidthCheck : public DkaqClangTidyCheck {
public:
  BitFieldWidthCheck(StringRef Name, ClangTidyContext *Context)
      : DkaqClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};



}
} // namespace tidy 
} // namespace clang 