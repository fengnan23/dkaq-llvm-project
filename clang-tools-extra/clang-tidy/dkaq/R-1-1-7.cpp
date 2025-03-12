//===--- MacroParamParenthesesCheck.cpp - clang-tidy ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "R-1-1-7.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Lexer.h"
#include "clang/Lex/MacroArgs.h"
#include "clang/Lex/MacroInfo.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"
#include <unordered_map>
#include <unordered_set>


namespace clang {
namespace tidy {
namespace dkaq {

// 请给clang-tidy增加一个检查项,希望通过修改clang-tidy的源代码,增加一个C++的功能检查. 如何用define定义的函数,define的结果中的变量必须用括号括起来. 例如#define abs(x) x>=0?x:-x.就是错误的,其中函数内容中的x应该用(x)
namespace {
class MacroParamParenthesesCallback : public PPCallbacks {
public:
  explicit MacroParamParenthesesCallback(MacroParamParenthesesCheck &Check,
                                       Preprocessor &PP)
      : Check(Check), PP(PP), SM(PP.getSourceManager()) {}

  void MacroDefined(const Token &MacroNameTok,
                   const MacroDirective *MD) override {
    // Skip if the macro is from system headers
    if (SM.isInSystemHeader(MacroNameTok.getLocation()))
      return;
    
    const MacroInfo *MI = MD->getMacroInfo();
    
    // Only look at function-like macros with parameters
    if (!MI->isFunctionLike() || MI->getNumParams() == 0)
      return;
    
    // Check each parameter usage
    for (unsigned i = 0; i < MI->getNumParams(); ++i) {
      const IdentifierInfo *ParamId = MI->params()[i];
      checkParamUsage(MacroNameTok, MI, ParamId, i);
    }
  }

private:
  void checkParamUsage(const Token &MacroNameTok, const MacroInfo *MI,
                      const IdentifierInfo *ParamId, unsigned ParamIndex) {
    StringRef ParamName = ParamId->getName();
    
    // Collect token locations where this parameter is used
    std::vector<unsigned> ParamUseLocations;
    
    // Analyze the macro's body to find parameter uses
    for (unsigned i = 0; i < MI->getNumTokens(); ++i) {
      const Token &Tok = MI->getReplacementToken(i);
      if (Tok.getKind() == tok::identifier && 
          Tok.getIdentifierInfo() == ParamId) {
        ParamUseLocations.push_back(i);
      }
    }
    
    // For each use of the parameter, check if it's properly parenthesized
    for (unsigned Loc : ParamUseLocations) {
      // Look for surrounding parentheses
      bool HasOpenParen = false;
      bool HasCloseParen = false;
      
      // Check for opening parenthesis before parameter
      if (Loc > 0) {
        const Token &PrevTok = MI->getReplacementToken(Loc - 1);
        HasOpenParen = PrevTok.getKind() == tok::l_paren;
      }
      
      // Check for closing parenthesis after parameter
      if (Loc < MI->getNumTokens() - 1) {
        const Token &NextTok = MI->getReplacementToken(Loc + 1);
        HasCloseParen = NextTok.getKind() == tok::r_paren;
      }
      
      // If the parameter is not properly parenthesized, report a warning
      if (!HasOpenParen || !HasCloseParen) {
        // Get the source location for the diagnostic
        SourceLocation MacroLoc = MacroNameTok.getLocation();
        
        // Create a helpful diagnostic message
        std::string Message = "违反GJB R-1-1-7 宏定义函数中的参数 '" + ParamName.str() + 
                            "' 一定要试用括号. 由DKAQ提供";
                            
        // Get the text of the macro definition for the note
        SourceRange DefRange(MI->getDefinitionLoc(), 
                           MI->getDefinitionEndLoc());
        
        // Report the issue
        auto Diag = Check.diag(MacroLoc, Message);
        
        // Add a fix-it hint to insert parentheses around the parameter
        /*SourceLocation ParamLoc = Tok.getLocation();
        if (!HasOpenParen) {
          Diag << FixItHint::CreateInsertion(ParamLoc, "(");
        }
        if (!HasCloseParen) {
          SourceLocation AfterParamLoc = 
            Lexer::getLocForEndOfToken(ParamLoc, 0, SM, PP.getLangOpts());
          Diag << FixItHint::CreateInsertion(AfterParamLoc, ")");
        }*/
      }
    } // end for
  }

  MacroParamParenthesesCheck &Check;
  Preprocessor &PP;
  const SourceManager &SM;
};
} // namespace

void MacroParamParenthesesCheck::registerPPCallbacks(
    const SourceManager &SM, Preprocessor *PP, Preprocessor *ModuleExpanderPP) {
  PP->addPPCallbacks(std::make_unique<MacroParamParenthesesCallback>(*this, *PP));
}

} // namespace dkaq
} // namespace tidy
} // namespace clang