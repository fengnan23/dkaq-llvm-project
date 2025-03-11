#pragma once

#include "clang/Basic/Diagnostic.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "clang/AST/RecursiveASTVisitor.h"

#include "../ClangTidy.h"
#include "../ClangTidyCheck.h"
 
using namespace clang;
using namespace clang::tidy;
 
//我在给clang-tidy增加一个检查项,希望通过修改clang-tidy的源代码,增加一个C++的功能检查. 检查在一个函数中呢,#define和#undef必须在同一个函数中成对出现.其中的undef必须和define在一个函数体内.如果undef不在函数体内,报错
class DefineUndefPairCheck : public ClangTidyCheck {
public:
  DefineUndefPairCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
 
  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override {
    PP->addPPCallbacks(std::make_unique<DefineUndefPairPPCallbacks>(this, SM));
  }
 
  void registerMatchers(ast_matchers::MatchFinder *Finder) override {
    Finder->addMatcher(
        ast_matchers::functionDecl().bind("function"),
        this);
  }
 
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override {
    if (const auto *Func = Result.Nodes.getNodeAs<FunctionDecl>("function"))  {
      // 在函数结束时检查未配对的#define 
      for (const auto &Macro : ActiveMacros) {
        diag(Macro.getValue(),  "Macro '%0' is defined but not undefined in function")
            << Macro.getKey(); 
      }
      ActiveMacros.clear();  // 清空当前函数的宏记录 
    }
  }
 
private:
  class DefineUndefPairPPCallbacks : public PPCallbacks {
  public:
    DefineUndefPairPPCallbacks(DefineUndefPairCheck *Check, const SourceManager &SM)
        : Check(Check), SM(SM) {}
 
    void MacroDefined(const Token &MacroNameTok, const MacroDirective *MD) override {
      StringRef MacroName = MacroNameTok.getIdentifierInfo()->getName(); 
      // 记录当前宏定义的位置 
      Check->ActiveMacros[MacroName] = MacroNameTok.getLocation(); 
    }
 
    void MacroUndefined(const Token &MacroNameTok, const MacroDefinition &MD,
                        const MacroDirective *Undef) override {
      StringRef MacroName = MacroNameTok.getIdentifierInfo()->getName(); 
      // 检查是否有对应的#define 
      if (!Check->ActiveMacros.erase(MacroName))  {
        Check->diag(MacroNameTok.getLocation(), 
                    "Macro '%0' is undefined without a corresponding #define in function")
            << MacroName;
      }
    }
 
  private:
    DefineUndefPairCheck *Check;
    const SourceManager &SM;
  };
 
  llvm::StringMap<SourceLocation> ActiveMacros; // 记录当前函数中未配对的#define 
};