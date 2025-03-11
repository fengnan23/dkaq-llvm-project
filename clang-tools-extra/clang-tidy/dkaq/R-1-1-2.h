#include "clang/Basic/Diagnostic.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"
#include "../ClangTidy.h"
#include "../ClangTidyCheck.h"

using namespace clang;
using namespace clang::tidy;

//我在给clang-tidy增加一个检查项,希望通过修改clang-tidy的源代码,增加一个C++的功能检查. 希望每一个define的内容,不能是c++的关键字的用法.例如:define LONG1 if,因为if是C++的关键字,所以这个需要报错.
 
class DefineValueKeywordCheck : public ClangTidyCheck {
public:
  DefineValueKeywordCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
 
  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override {
    PP->addPPCallbacks(std::make_unique<DefineValueKeywordPPCallbacks>(this));
  }
 
private:
  class DefineValueKeywordPPCallbacks : public PPCallbacks {
  public:
    DefineValueKeywordPPCallbacks(DefineValueKeywordCheck *Check) : Check(Check) {}
 
    void MacroDefined(const Token &MacroNameTok, const MacroDirective *MD) override {
      // 获取宏定义的值部分 
      const MacroInfo *MI = MD->getMacroInfo();
      for (const Token &Tok : MI->tokens()) {
        StringRef TokenText = Tok.getIdentifierInfo()  ? Tok.getIdentifierInfo()->getName()  : "";
        if (isCppKeyword(TokenText)) {
          Check->diag(Tok.getLocation(), 
                      "违反GJB R-1-1-2 Macro value '%0' is a C++ keyword")
              << TokenText;
        }
      }
    }
 
  private:
    DefineValueKeywordCheck *Check;
 
    bool isCppKeyword(StringRef Name) {
      static const llvm::StringSet<> Keywords = {
          "if", "else", "while", "for", "return", "class", "int", "long", "switch", "case"};
      return Keywords.contains(Name); 
    }
  };
};