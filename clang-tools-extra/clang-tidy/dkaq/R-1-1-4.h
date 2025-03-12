#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "../ClangTidy.h"
#include "../ClangTidyCheck.h"
 
using namespace clang;
using namespace clang::tidy;

//我在给clang-tidy增加一个检查项,希望通过修改clang-tidy的源代码,增加一个C++的功能检查. 检查不能定义一个c++默认的关键字为变量. 例如: unsigned int cout = 0

class KeywordVariableCheck : public ClangTidyCheck {
public:
  KeywordVariableCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
 
  void registerMatchers(ast_matchers::MatchFinder *Finder) override {
    Finder->addMatcher(
        ast_matchers::varDecl().bind("var"),
        this);
  }
 
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override {
    if (const auto *Var = Result.Nodes.getNodeAs<VarDecl>("var"))  {
      StringRef VarName = Var->getName();
      if (isCppKeyword(VarName)) {
        diag(Var->getLocation(), "违反GJB R-1-1-4 变量名称 '%0' 是c++关键字. 由DKAQ提供")
            << VarName;
      }
    }
  }
 
private:
  bool isCppKeyword(StringRef Name) {
    static const llvm::StringSet<> Keywords = {
        "cout", "cin", "int", "unsigned", "if", "else", "while", "for", "return", "class", "cout", "cin", "endl"};
    return Keywords.contains(Name); 
  }
};