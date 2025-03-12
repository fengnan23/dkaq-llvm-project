//===--- NamedDeclarationCheck.cpp - clang-tidy ---------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "R-1-1-8.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace dkaq {
//请给clang-tidy增加一个检查项,希望通过修改clang-tidy的源代码,增加一个C++的功能检查.希望每一个结构/枚举/联合的定义都有标识名. 否则报错

void NamedDeclarationCheck::registerMatchers(MatchFinder *Finder) {
  // Match struct declarations without tags (names)
  Finder->addMatcher(
      recordDecl(isStruct(),
                unless(isImplicit()),
                unless(has(tagDecl())))
          .bind("anonymousStruct"),
      this);
  
  // Match enum declarations without tags
  Finder->addMatcher(
      enumDecl(unless(isImplicit()),
              unless(has(tagDecl())))
          .bind("anonymousEnum"),
      this);
  
  // Match union declarations without tags
  Finder->addMatcher(
      recordDecl(isUnion(),
                unless(isImplicit()),
                unless(has(tagDecl())))
          .bind("anonymousUnion"),
      this);
}

void NamedDeclarationCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedStruct = Result.Nodes.getNodeAs<RecordDecl>("anonymousStruct");
  const auto *MatchedEnum = Result.Nodes.getNodeAs<EnumDecl>("anonymousEnum");
  const auto *MatchedUnion = Result.Nodes.getNodeAs<RecordDecl>("anonymousUnion");
  
  // Don't flag declarations in system headers
  if ((MatchedStruct && MatchedStruct->getLocation().isInvalid()) ||
      (MatchedEnum && MatchedEnum->getLocation().isInvalid()) ||
      (MatchedUnion && MatchedUnion->getLocation().isInvalid()))
    return;
  
  // Don't flag C++11 anonymous structs or unions inside other structs
  // These are useful for composition
  if ((MatchedStruct && MatchedStruct->isAnonymousStructOrUnion()) ||
      (MatchedUnion && MatchedUnion->isAnonymousStructOrUnion()))
    return;

  const SourceManager &SM = *Result.SourceManager;
  
  if (MatchedStruct) {
    // Skip if in system header
    if (SM.isInSystemHeader(MatchedStruct->getLocation()))
      return;
      
    // Skip if this is a lambda
    if (MatchedStruct->isLambda())
      return;

    diag(MatchedStruct->getLocation(), 
        "违反GJB-R-1-1-8 结构体定义必须要有名字. 由DKAQ提供");
  }
  
  if (MatchedEnum) {
    // Skip if in system header
    if (SM.isInSystemHeader(MatchedEnum->getLocation()))
      return;

    diag(MatchedEnum->getLocation(), 
        "违反GJB-R-1-1-8 枚举类型定义必须要有名字. 由DKAQ提供");
  }
  
  if (MatchedUnion) {
    // Skip if in system header
    if (SM.isInSystemHeader(MatchedUnion->getLocation()))
      return;

    diag(MatchedUnion->getLocation(), 
        "违反GJB R-1-1-8 联合类型定义必须要有名字. 由DKAQ提供");
  }
}

} // namespace dkaq
} // namespace tidy
} // namespace clang