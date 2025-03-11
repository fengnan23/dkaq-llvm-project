//===--- NamedParametersCheck.cpp - clang-tidy ---------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "R-1-1-13.h"

#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace dkaq {

//请给clang-tidy增加一个检查项,希望通过修改clang-tidy的源代码,增加一个C++的功能检查.希望检查每一个函数的定义,必须都给出变量名.例如: int func(int,int);就是错误的,函数参数没有变量名.
void NamedParametersCheck::registerMatchers(MatchFinder *Finder) {
  // Match function declarations with at least one parameter
  Finder->addMatcher(
      functionDecl(hasAnyParameter(parmVarDecl())).bind("funcDeclareWithParam"),
      this);
}

void NamedParametersCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *FuncDecl = Result.Nodes.getNodeAs<FunctionDecl>("funcDeclareWithParam");
  if (!FuncDecl)
    return;
    
  // Skip if this function is in a system header
  if (Result.SourceManager->isInSystemHeader(FuncDecl->getLocation()))
    return;
    
  // Skip main() function
  if (FuncDecl->isMain())
    return;
    
  // Skip implicitly-generated functions
  if (FuncDecl->isImplicit())
    return;

  // Check each parameter
  for (unsigned i = 0; i < FuncDecl->getNumParams(); ++i) {
    const ParmVarDecl *Param = FuncDecl->getParamDecl(i);
    
    // Skip if parameter is unnamed and function has a body
    // This is likely a function definition where parameter name is optional
    if (Param->getName().empty()) {
      std::string ParamType = Param->getType().getAsString();
      
      auto Diag = diag(Param->getLocation(), 
                      "违反GJB R-1-1-13 function parameter #%0 of type '%1' is unnamed")
                   << (i + 1) << ParamType;
      
      // Suggest a default parameter name based on type
      std::string SuggestedName = "param" + std::to_string(i + 1);
      
      // If we can identify the type, suggest a more meaningful name
      std::string TypeStr = Param->getType().getAsString();
      if (TypeStr.find("int") != std::string::npos)
        SuggestedName = "value" + std::to_string(i + 1);
      else if (TypeStr.find("char") != std::string::npos)
        SuggestedName = "c" + std::to_string(i + 1);
      else if (TypeStr.find("bool") != std::string::npos)
        SuggestedName = "flag" + std::to_string(i + 1);
      else if (TypeStr.find("string") != std::string::npos)
        SuggestedName = "str" + std::to_string(i + 1);
      else if (TypeStr.find("double") != std::string::npos || 
               TypeStr.find("float") != std::string::npos)
        SuggestedName = "val" + std::to_string(i + 1);
      else if (TypeStr.find("*") != std::string::npos)
        SuggestedName = "ptr" + std::to_string(i + 1);
      
      // Add fix-it hint
      Diag << FixItHint::CreateInsertion(Param->getLocation().getLocWithOffset(
                                         Param->getType().getAsString().length()),
                                         " " + SuggestedName);
    }
  }
}

} // namespace dkaq
} // namespace tidy
} // namespace clang