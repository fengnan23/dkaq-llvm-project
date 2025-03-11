// MyUnusedVarCheck.cpp
#include "UnusedVarDiagnostic.h"


using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace dkaq {

void MyUnusedVarCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(varDecl().bind("unusedVar"), this);
}

void MyUnusedVarCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Var = Result.Nodes.getNodeAs<VarDecl>("unusedVar");
  if (Var->isLocalVarDecl() && !Var->isUsed()) {
    diag(Var->getLocation(), "违反GJB D-0-0-1 unused variable '%0'. Powered by DKAQ") << Var->getNameAsString();
  }
}

} // namespace dkaq

} // namespace tidy
} // namespace clang