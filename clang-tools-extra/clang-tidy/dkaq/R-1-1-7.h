#pragma once

#include "clang/Basic/Diagnostic.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"

#include "../ClangTidy.h"
#include "../ClangTidyCheck.h"

 
using namespace clang;
using namespace clang::tidy;

namespace clang {
namespace tidy {
namespace dkaq {

/// Checks that parameters in function-like macros are properly wrapped in 
/// parentheses when used in the replacement text.
///
/// For example:
/// \code
///   #define SQUARE(x) x*x       // Bad: should be (x)*(x)
///   #define SQUARE(x) (x)*(x)   // Good
/// \endcode
class MacroParamParenthesesCheck : public ClangTidyCheck {
public:
  MacroParamParenthesesCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                          Preprocessor *ModuleExpanderPP) override;
};

} // namespace dkaq
} // namespace tidy
} // namespace clang

