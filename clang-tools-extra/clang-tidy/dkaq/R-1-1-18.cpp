//===--- ExplicitArrayBoundsCheck.cpp - clang-tidy -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "R-1-1-18.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Lexer.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"
#include "../utils/Matchers.h"
#include "../utils/OptionsUtils.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace dkaq {
//请给clang-tidy增加一个检查项,希望通过修改clang-tidy的源代码,增加一个C++的功能检查.希望检查每一个数组的定义，都有明确的显示的边界定义。例如：int a[]={0,1,2}就是错误的

void ExplicitArrayBoundsCheck::registerMatchers(MatchFinder *Finder) {
  // Match variable declarations of array type with implicit size
  Finder->addMatcher(
      varDecl(
          hasType(arrayType().bind("arrayType")),
          unless(parmVarDecl()) // Don't match function parameters
      ).bind("arrayDecl"),
      this);
}

void ExplicitArrayBoundsCheck::check(const MatchFinder::MatchResult &Result) {  
  const auto *Var = Result.Nodes.getNodeAs<VarDecl>("arrayDecl");   
  if (!Var) return;  
 
  // 获取数组类型在源码中的定义信息  
  TypeSourceInfo *TypeSource = Var->getTypeSourceInfo();  
  if (!TypeSource) return;
  
  const Expr *Init = Var->getInit();
  // If it's a string literal for a char array, skip it (common idiom)
  if (isa<StringLiteral>(Init->IgnoreImplicit()) && 
     Var->getType()->getArrayElementTypeNoTypeQual()->isCharType())
     return;
 
  // 遍历类型定位信息，找到数组边界定义  
  TypeLoc TypeLoc = TypeSource->getTypeLoc();
  while (TypeLoc) {  
    if (auto ArrayLoc = TypeLoc.getAs<ArrayTypeLoc>())  {  
      // 检查是否显式指定了数组大小  
      if (!ArrayLoc.getSizeExpr())  {  
        diag(Var->getLocation(), "违反GJB R-1-1-18 '%0'数组定义必须显式声明边界")  
            << Var->getName();  
      }  
      break;  
    }  
    TypeLoc = TypeLoc.getNextTypeLoc();   
  }  
} 

} // namespace dkaq
} // namespace tidy
} // namespace clang