// VirtualDestructorCheck.h 
#ifndef CLANG_TIDY_MISC_VIRTUALDESTRUCTORCHECK_H 
#define CLANG_TIDY_MISC_VIRTUALDESTRUCTORCHECK_H 
 
#include "../ClangTidy.h"
#include "../ClangTidyCheck.h"
 
namespace clang {
namespace tidy {
namespace dkaq {
 
class VirtualDestructorCheck : public ClangTidyCheck {
public:
  VirtualDestructorCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
 
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
}; // end of class
  
} // namespace dkaq
} // namespace tidy 
} // namespace clang 
 
#endif // CLANG_TIDY_MISC_VIRTUALDESTRUCTORCHECK_H 
