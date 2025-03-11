
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/Basic/Diagnostic.h"
#include "../ClangTidyModule.h"
#include "../ClangTidyModuleRegistry.h"

#include "VirtualDestructorCheck.h"
#include "UnusedVarDiagnostic.h"

#include "R-1-1-1.h"
#include "R-1-1-2.h"
#include "R-1-1-3.h"
#include "R-1-1-4.h"
#include "R-1-1-5.h"
//#include "R-1-1-6.h" // 无法实现, define在函数的分析前面完成
#include "R-1-1-7.h"
#include "R-1-1-8.h"
//#include "R-1-1-9.h" // 无需实现,和R-1-1-8一样的功能,已经在R-1-1-8中实现了
#include "R-1-1-10.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace dkaq {

//我在给clang-tidy增加一个检查项,希望通过修改clang-tidy的源代码,增加一个C++的功能检查. 检查不能定义一个#define的条件,不能在没有#undef之前再#define成一个新的值

class DkaqModule : public ClangTidyModule {
 public:
  void addCheckFactories(ClangTidyCheckFactories &CheckFactories) override {
    CheckFactories.registerCheck<MyUnusedVarCheck>("dkaq-unused-variables");
	CheckFactories.registerCheck<VirtualDestructorCheck>("dkaq-virtual-destructor");
	CheckFactories.registerCheck<DefineKeywordCheck>("dkaq-define-keyword"); 
	CheckFactories.registerCheck<DefineValueKeywordCheck>("dkaq-define-value");
	CheckFactories.registerCheck<TypedefRedefinitionCheck>("dkaq-typedef-duplicate");
	CheckFactories.registerCheck<KeywordVariableCheck>("dkaq-keyword-variable");
	CheckFactories.registerCheck<DefineRedefinitionCheck>("dkaq-redefine");
	//CheckFactories.registerCheck<DefineUndefPairCheck>("dkaq-unpair-define-function"); // R-1-1-6
	CheckFactories.registerCheck<MacroParamParenthesesCheck>("dkaq-marco-parentheses");
	CheckFactories.registerCheck<NamedDeclarationCheck>("dkaq-unnamed-union-struct-declaration");
	CheckFactories.registerCheck<BitFieldWidthCheck>("dkaq-bitfield-unsign-check");
  }
}; 

// Register the GoogleTidyModule using this statically initialized variable.
static ClangTidyModuleRegistry::Add<DkaqModule> X("dkaq-module",
                                                    "Adds dkaq GJB checks.");

// This anchor is used to force the linker to link in the generated object file
// and thus register the GoogleModule.
} // namespace dkaq

volatile int DkaqModuleAnchorSource = 0;
} // namespace tidy
} // namespace clang