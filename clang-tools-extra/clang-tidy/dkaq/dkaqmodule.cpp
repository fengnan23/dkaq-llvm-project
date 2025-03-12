
#include "../ClangTidyModule.h"
#include "../ClangTidyModuleRegistry.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/Basic/Diagnostic.h"

#include "UnusedVarDiagnostic.h"
#include "VirtualDestructorCheck.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "R-1-1-1.h"
#include "R-1-1-2.h"
#include "R-1-1-3.h"
#include "R-1-1-4.h"
#include "R-1-1-5.h"
// #include "R-1-1-6.h" // 无法实现, define在函数的分析前面完成
#include "R-1-1-7.h"
#include "R-1-1-8.h"
// #include "R-1-1-9.h" // 无需实现,和R-1-1-8一样的功能,已经在R-1-1-8中实现了
#include "R-1-1-10.h"
#include "R-1-1-11.h"
// #include "R-1-1-12.h" // 这个标准没有看懂,应该不正确
#include "R-1-1-13.h"
// #include "R-1-1-14.h" // 这个标准应该是错误的，无法实现函数重载
#include "R-1-1-15.h" // 这个在C++标准中就是error，编译不通过ERROR
// R-1-1-16
// 单个文件无法实现，外部变量名称，类型定义必须一致。如果不一致，link错误
#include "R-1-1-17.h"
#include "R-1-1-18.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace dkaq {

int getDaysSinceCompilation();
std::string g_strLicenseMsg;
const int MAX_DAYS_EXPIRE = 183;

class DkaqModule : public ClangTidyModule {
public:
  void addCheckFactories(ClangTidyCheckFactories &CheckFactories) override {
    int nDaysLeft = MAX_DAYS_EXPIRE-getDaysSinceCompilation();

    if (nDaysLeft <= 0)
      return;
    if (nDaysLeft <= MAX_DAYS_EXPIRE)
      g_strLicenseMsg = "版权" + std::to_string(nDaysLeft) + "天过期.";

    // CheckFactories.registerCheck<MyUnusedVarCheck>("dkaq-unused-variables");
    CheckFactories.registerCheck<VirtualDestructorCheck>(
        "dkaq-virtual-destructor");
    CheckFactories.registerCheck<DefineKeywordCheck>("dkaq-define-keyword");
    CheckFactories.registerCheck<DefineValueKeywordCheck>("dkaq-define-value");
    CheckFactories.registerCheck<TypedefRedefinitionCheck>(
        "dkaq-typedef-duplicate");
    CheckFactories.registerCheck<KeywordVariableCheck>("dkaq-keyword-variable");
    CheckFactories.registerCheck<DefineRedefinitionCheck>("dkaq-redefine");
    // CheckFactories.registerCheck<DefineUndefPairCheck>("dkaq-unpair-define-function");
    // // R-1-1-6
    CheckFactories.registerCheck<MacroParamParenthesesCheck>(
        "dkaq-marco-parentheses");
    CheckFactories.registerCheck<NamedDeclarationCheck>(
        "dkaq-unnamed-union-struct-declaration");
    CheckFactories.registerCheck<BitFieldWidthCheck>(
        "dkaq-bitfield-unsign"); // R-1-1-10
    CheckFactories.registerCheck<ExplicitBitFieldSignCheck>(
        "dkaq-bitfield-explicit-sign"); // R-1-1-11
    // // R-1-1-12
    CheckFactories.registerCheck<NamedParametersCheck>(
        "dkaq-bitfield-explicit-sign-check"); // R-1-1-13
    // // R-1-1-14
    CheckFactories.registerCheck<ExplicitParameterTypeCheck>(
        "dkaq-func-explicit-param-name"); // R-1-1-15
    // // R-1-1-16
    CheckFactories.registerCheck<NoExternInFunctionCheck>(
        "dkaq-extern-inside-function"); // R-1-1-17
    CheckFactories.registerCheck<ExplicitArrayBoundsCheck>(
        "dkaq-array-without-explicit-boundary"); // R-1-1-18

    if (MAX_DAYS_EXPIRE - getDaysSinceCompilation() <= 0)
      // Remove All checks from factories
      g_strLicenseMsg = "版权" + std::to_string(nDaysLeft) + "天过期.";   
  }
};

DiagnosticBuilder DkaqClangTidyCheck::diag(SourceLocation Loc, StringRef Description,
                       DiagnosticIDs::Level Level) {
  if (g_strLicenseMsg != "") {
    std::string modifiedDescription = g_strLicenseMsg + " " + Description.str();
    return ClangTidyCheck::diag(Loc, modifiedDescription, Level);
  }
  return ClangTidyCheck::diag(Loc, Description, Level);
}

// Register the GoogleTidyModule using this statically initialized variable.
static ClangTidyModuleRegistry::Add<DkaqModule> X("dkaq-module",
                                                  "Adds dkaq GJB checks.");

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
// This anchor is used to force the linker to link in the generated object file
// and thus register the GoogleModule.
} // namespace dkaq

volatile int DkaqModuleAnchorSource = 0;



} // namespace tidy
} // namespace clang