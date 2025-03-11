//===--- NoExternInFunctionCheck.cpp - clang-tidy ------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "R-1-1-17.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/AST/Decl.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchersMacros.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/Basic/Specifiers.h"
#include "clang/Lex/Token.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace dkaq {
	
AST_POLYMORPHIC_MATCHER(isExternStorageClass,
                        AST_POLYMORPHIC_SUPPORTED_TYPES(FunctionDecl,
                                                        VarDecl)) {
  return Node.getStorageClass() == SC_Extern;
}

////请给clang-tidy增加一个检查项,希望通过修改clang-tidy的源代码,增加一个C++的功能检查.希望检查每一个extern的变量，不能在一个函数体内声明
void NoExternInFunctionCheck::registerMatchers(MatchFinder *Finder) {
  // Match variable declarations that:
  // 1. Have the "extern" storage class specifier
  // 2. Are located inside a function body
  Finder->addMatcher(
      varDecl(
          isExternStorageClass(),
		  hasAncestor(functionDecl())
      ).bind("externVar"),
      this);
}

void NoExternInFunctionCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *ExternVar = Result.Nodes.getNodeAs<VarDecl>("externVar");
  if (!ExternVar)
 
  // Skip if in system headers
  if (Result.SourceManager->isInSystemHeader(ExternVar->getLocation()))
    return;

  
  // Get variable and function names for the diagnostic
  std::string VarName = ExternVar->getNameAsString(); 
  
  // Issue diagnostic
  auto Diag = diag(ExternVar->getLocation(), 
                  "违反GJB R-1-1-17 extern variable '%0' should not be declared inside function")
                << VarName;
  
}

} // namespace dkaq
} // namespace tidy
} // namespace clang