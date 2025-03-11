

#include "VirtualDestructorCheck.h"
 
namespace clang {
namespace tidy {
namespace dkaq {
 
 void VirtualDestructorCheck::registerMatchers(ast_matchers::MatchFinder *Finder) {
    using namespace ast_matchers;
    Finder->addMatcher(
        cxxRecordDecl(
            isDefinition(),
            unless(isImplicit()),
            unless(isLambda()),
            hasMethod(isVirtual())
        ).bind("class"),
        this 
    );
  }
  
  void VirtualDestructorCheck::check(const ast_matchers::MatchFinder::MatchResult &Result) {
    const auto *Class = Result.Nodes.getNodeAs<CXXRecordDecl>("class"); 
    if (!Class) return;
 
    const CXXDestructorDecl *Dtor = Class->getDestructor();
    if (!Dtor || !Dtor->isVirtual()) {
      diag(Dtor->getBeginLoc(), 
           " 违反GJB D-1-0-1 Virtual Destructor. Powered by DKAQ");
    }
  }

} // namespace dkaq
} // namespace tidy 
} // namespace clang 