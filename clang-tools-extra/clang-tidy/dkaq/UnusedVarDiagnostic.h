// MyUnusedVarCheck.h
#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MYCHECKS_MYUNUSEDVARCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MYCHECKS_MYUNUSEDVARCHECK_H

#include "../ClangTidy.h"
#include "../ClangTidyCheck.h"

namespace clang {
namespace tidy {
namespace dkaq {

class MyUnusedVarCheck : public ClangTidyCheck {
public:
  MyUnusedVarCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace mychecks
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MYCHECKS_MYUNUSEDVARCHECK_H