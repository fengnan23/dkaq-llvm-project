#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "clang/AST/RecursiveASTVisitor.h"

#include "../ClangTidy.h"
#include "../ClangTidyCheck.h"

 
using namespace clang;
using namespace clang::tidy;
using namespace llvm;
 
 //我在给clang-tidy增加一个检查项,希望通过修改clang-tidy的源代码,增加一个C++的功能检查. 希望每一个typedef的内容不能被重复定义,例如:typedef int mytype; typedef float mytype. 这里的mytype被重复定义,即使这两个typedef在不同的函数作用域中,也需要报错.
 
class TypedefRedefinitionCheck : public ClangTidyCheck {
public:
   TypedefRedefinitionCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {
  }
 
  void registerMatchers(ast_matchers::MatchFinder *Finder) override {
    Finder->addMatcher(
        ast_matchers::typedefDecl().bind("typedef"),
        this);
  }
 
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override {
    if (const auto *Typedef = Result.Nodes.getNodeAs<TypedefDecl>("typedef"))  {
      StringRef TypedefName = Typedef->getName();
      const ASTContext *Context = Result.Context;
 
      // 遍历整个翻译单元，检查是否已经存在同名typedef 
      for (const auto *Decl : Context->getTranslationUnitDecl()->decls()) {
        if (const auto *OtherTypedef = dyn_cast<TypedefDecl>(Decl)) {
          if (OtherTypedef != Typedef && OtherTypedef->getName() == TypedefName) {
            diag(Typedef->getLocation(), "违反GJB R-1-1-3 重复typedef定义 '%0'. 由DKAQ提供")
                << TypedefName;
            diag(OtherTypedef->getLocation(), "前面的定义在这里. 由DKAQ提供",
                 DiagnosticIDs::Note);
            break;
          }
        }
      }
    }
  }
};