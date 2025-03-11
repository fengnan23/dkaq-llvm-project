//===--- ExplicitParameterTypeCheck.cpp - clang-tidy --------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "R-1-1-15.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Basic/DiagnosticIDs.h"
#include "clang/Lex/Lexer.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace dkaq {
////请给clang-tidy增加一个检查项,希望通过修改clang-tidy的源代码,增加一个C++的功能检查.希望检查每一个函数的参数必须都有类型声明。例如：int func(height)是错误的，必须是 int func(int height)

void ExplicitParameterTypeCheck::registerMatchers(MatchFinder *Finder) {
  // Match all function declarations
  Finder->addMatcher(
      functionDecl().bind("ExplicitParameterTypeCheck"),
      this);
}

void ExplicitParameterTypeCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *FuncDecl = Result.Nodes.getNodeAs<FunctionDecl>("ExplicitParameterTypeCheck");
  if (!FuncDecl)
    return;
    
  // Skip if in system headers
  if (Result.SourceManager->isInSystemHeader(FuncDecl->getLocation()))
    return;
  
  // Skip implicitly-generated functions
  if (FuncDecl->isImplicit())
    return;

  // Skip C++11 default/delete constructors
  if (FuncDecl->isDeleted() || FuncDecl->isDefaulted())
    return;

  // Check if this is a K&R-style function declaration
  if (FuncDecl->hasWrittenPrototype()) // Modern C++ style with types in prototype
    return;

  const SourceManager &SM = *Result.SourceManager;
  
  // K&R-style declarations without parameter types are not valid C++
  // but we'll check them anyway
  for (const ParmVarDecl *Param : FuncDecl->parameters()) {
    if (!Param->getTypeSourceInfo() || 
        Param->getTypeSourceInfo()->getTypeLoc().isNull()) {
      // This parameter doesn't have a proper type declaration
      
      std::string ParamName = Param->getNameAsString();
      if (!ParamName.empty()) {
        // Get the function name for the diagnostic
        std::string FuncName = FuncDecl->getNameAsString();
        
        // Issue diagnostic
        diag(Param->getLocation(), 
            "parameter '%0' in function '%1' has no type specifier")
            << ParamName << FuncName;
        
        // Can't really provide a meaningful fix-it hint without knowing
        // the intended type, but we can give a note
        diag(FuncDecl->getLocation(), 
            "consider providing explicit types for all parameters",
            DiagnosticIDs::Note);
      }
    }
  }
}

} // namespace dkaq
} // namespace tidy
} // namespace clang