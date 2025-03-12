//===--- ExplicitBitFieldSignCheck.cpp - clang-tidy ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
#include "R-1-1-11.h"

#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/Lexer.h"  // Add this include for Lexer::getSourceText

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace dkaq {

//请给clang-tidy增加一个检查项,希望通过修改clang-tidy的源代码,增加一个C++的功能检查.希望检查每一个类/struct/union中,如果有类型位整形的位,必须明确定义signed或者unsigned
void ExplicitBitFieldSignCheck::registerMatchers(MatchFinder *Finder) {
  // Match bit-fields with integer types
  Finder->addMatcher(
      fieldDecl(
          isBitField(), // Match bit-fields
          anyOf(
			hasType(asString("int")),
			hasType(asString("short"))
		)
      ).bind("integerExplicitBitField"),
      this);
}

void ExplicitBitFieldSignCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Field = Result.Nodes.getNodeAs<FieldDecl>("integerExplicitBitField");
  if (!Field)
    return;
    
  // Skip if in system headers
  if (Result.SourceManager->isInSystemHeader(Field->getLocation()))
    return;
  
  // Get the type
  QualType FieldType = Field->getType();
  const Type *Type = FieldType.getTypePtr();
  
  // Check if it's a built-in type
  if (!Type->isBuiltinType())
    return;
    
  const BuiltinType *BuiltinTy = Type->getAs<BuiltinType>();
  if (!BuiltinTy)
    return;
  
  // Look at the type specifier tokens from the source
  // to check if 'signed' or 'unsigned' was explicitly specified
  const TypeSourceInfo *TSI = Field->getTypeSourceInfo();
  if (!TSI)
    return;
    
  // Get the type location and source
  TypeLoc TL = TSI->getTypeLoc();
  SourceLocation TypeLoc = TL.getBeginLoc();
  
  // Get the source code at the location
  std::string TypeStr = Lexer::getSourceText(
      CharSourceRange::getTokenRange(TypeLoc, TL.getEndLoc()),
      *Result.SourceManager, 
      Result.Context->getLangOpts()).str();
  
  // Check if "signed" or "unsigned" is explicitly specified
  bool HasExplicitSign = 
    (TypeStr.find("signed") != std::string::npos) || 
    (TypeStr.find("unsigned") != std::string::npos);
    
  // Special case for 'int' which is implicitly signed but very common
  bool IsJustInt = (TypeStr == "int");
  
  // If no explicit sign was specified, issue a warning
  if (!HasExplicitSign) {
    auto Diag = 
        diag(Field->getLocation(), 
             "违反GJB R-1-1-11 位变量'%0'一定需要显式的符号定义,例如：unsigned. 由DKAQ提供")
        << Field->getName() << TypeStr;
    
    // Add a fix-it hint to add 'signed' or 'unsigned' based on the actual type
    if (IsJustInt) {
      if (BuiltinTy->isSignedInteger()) {
        Diag << FixItHint::CreateInsertion(TypeLoc, "signed ");
      } else if (BuiltinTy->isUnsignedInteger()) {
        Diag << FixItHint::CreateInsertion(TypeLoc, "unsigned ");
      }
    }
  }
}

} // namespace dkaq
} // namespace tidy
} // namespace clang