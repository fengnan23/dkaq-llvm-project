#pragma once

#include "clang/Basic/Diagnostic.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"
#include "../ClangTidy.h"
#include "../ClangTidyCheck.h"
 
using namespace clang;
using namespace clang::tidy;
 
//我在给clang-tidy增加一个检查项,希望通过修改clang-tidy的源代码,增加一个C++的功能检查. 检查不能定义一个#define的条件,不能在没有#undef之前再#define成一个新的值
class DefineRedefinitionCheck : public ClangTidyCheck {
public:
  DefineRedefinitionCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
 
  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override {
    PP->addPPCallbacks(std::make_unique<DefineRedefinitionPPCallbacks>(this));
  }
 
private:
  class DefineRedefinitionPPCallbacks : public PPCallbacks {
  public:
    DefineRedefinitionPPCallbacks(DefineRedefinitionCheck *Check) : Check(Check) {}
 
    void MacroDefined(const Token &MacroNameTok, const MacroDirective *MD) override {
      StringRef MacroName = MacroNameTok.getIdentifierInfo()->getName(); 
 
      // 检查是否已经存在同名宏定义 
      if (DefinedMacros.count(MacroName))  {
        Check->diag(MacroNameTok.getLocation(), 
                    "违反GJB R-1-1-5 Macro '%0' is redefined without #undef")
            << MacroName;
        Check->diag(DefinedMacros[MacroName], "Previous definition is here",
                    DiagnosticIDs::Note);
      }
 
      // 记录当前宏定义的位置 
      DefinedMacros[MacroName] = MacroNameTok.getLocation(); 
    }
 
    void MacroUndefined(const Token &MacroNameTok, const MacroDefinition &MD,
                        const MacroDirective *Undef) override {
      StringRef MacroName = MacroNameTok.getIdentifierInfo()->getName(); 
      // 移除已取消定义的宏 
      DefinedMacros.erase(MacroName); 
    }
 
  private:
    DefineRedefinitionCheck *Check;
    llvm::StringMap<SourceLocation> DefinedMacros; // 记录已定义的宏及其位置 
  };
};