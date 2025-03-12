#pragma once

#include "clang/Basic/Diagnostic.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"
#include "../ClangTidy.h"
#include "../ClangTidyCheck.h"


using namespace clang;
using namespace clang::tidy;

//我在给clang-tidy增加一个检查项,希望通过修改clang-tidy的源代码,增加一个C++的功能检查. 希望每一个define的内容,不能改变c++的关键字的用法.例如:define long 1,因为long是C++的关键字,所以这个需要报错.
class DefineKeywordCheck : public ClangTidyCheck {
public:
  DefineKeywordCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override {
    PP->addPPCallbacks(std::make_unique<DefineKeywordPPCallbacks>(this));
  }

private:
  class DefineKeywordPPCallbacks : public PPCallbacks {
  public:
    DefineKeywordPPCallbacks(DefineKeywordCheck *Check) : Check(Check) {}

    void MacroDefined(const Token &MacroNameTok, const MacroDirective *MD) override {
      StringRef MacroName = MacroNameTok.getIdentifierInfo()->getName(); 
      if (isCppKeyword(MacroName)) {
        Check->diag(MacroNameTok.getLocation(), 
                    "违反GJB R-1-1-1 重复define定义 '%0' 由DKAQ提供")
            << MacroName;
      }
    }

  private:
    DefineKeywordCheck *Check;

    bool isCppKeyword(StringRef Name) {
      static const llvm::StringSet<> Keywords = {
          "long", "int", "class", "return", "if", "else", "while", "for", "switch", "case"};
      return Keywords.contains(Name); 
    }
  };
};
