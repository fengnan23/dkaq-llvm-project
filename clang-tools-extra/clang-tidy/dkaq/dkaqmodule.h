#pragma once

#include <string>

namespace clang {
namespace tidy {
namespace dkaq {


using namespace clang;
using namespace clang::tidy;

class DkaqClangTidyCheck : public ClangTidyCheck {
public:
  DkaqClangTidyCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  DiagnosticBuilder diag(SourceLocation Loc, StringRef Description,
                         DiagnosticIDs::Level Level = DiagnosticIDs::Warning);
};

} // namespace dkaq
} // namespace tidy
} // namespace clang
