
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/Basic/Diagnostic.h"
#include "../ClangTidyModule.h"
#include "../ClangTidyModuleRegistry.h"

#include "VirtualDestructorCheck.h"
#include "UnusedVarDiagnostic.h"

#include <iostream>
#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>

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

/**
 * 计算当前日期与编译日期之间的天数差
 * 
 * @return 天数差，正值表示当前日期晚于编译日期
 */
int getDaysSinceCompilation() {
    // 获取编译日期 __DATE__ 格式为 "Mmm dd yyyy" (如 "Mar 11 2025")
    std::string compileDate = __DATE__;
    
    // 解析编译日期字符串
    std::tm compileTime = {};
    std::istringstream dateStream(compileDate);
    
    // 月份名称映射
    std::string monthName;
    dateStream >> monthName;
    
    std::string monthNames[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", 
                               "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
                               
    int month = 0;
    for (int i = 0; i < 12; i++) {
        if (monthName == monthNames[i]) {
            month = i;
            break;
        }
    }
    
    // 读取日和年
    int day, year;
    dateStream >> day >> year;
    
    // 设置编译时间结构
    compileTime.tm_year = year - 1900; // 年份从1900开始
    compileTime.tm_mon = month;        // 月份从0开始 (0-11)
    compileTime.tm_mday = day;
    compileTime.tm_hour = 0;
    compileTime.tm_min = 0;
    compileTime.tm_sec = 0;
    
    // 将编译时间转换为时间戳（秒）
    std::time_t compileTimeT = std::mktime(&compileTime);
    
    // 获取当前时间的时间戳
    std::time_t currentTimeT = std::time(nullptr);
    
    // 计算时间差（秒）
    double diffSeconds = std::difftime(currentTimeT, compileTimeT);
    
    // 转换为天数 (86400 = 秒/天)
    int diffDays = static_cast<int>(diffSeconds / 86400);
    
    return diffDays;
}

} // namespace tidy
} // namespace clang