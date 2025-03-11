#include "R-1-1-10.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/AST/Decl.h"
#include "clang/AST/Type.h"
#include "clang/Basic/SourceManager.h"
 
using namespace clang;
using namespace clang::tidy;
using namespace clang::ast_matchers;
 

 
namespace clang {
namespace tidy {
namespace dkaq {
 
/*void BitFieldWidthCheck::registerMatchers(MatchFinder *Finder) {
  // Match bit-fields that are of integer types
  Finder->addMatcher(
      fieldDecl(
          isBitField(),         // Match fields that are bit-fields
          hasType(isInteger())     // Match only integer types
      ).bind("bitfield"),
      this);
}*/

//请给clang-tidy增加一个检查项,希望通过修改clang-tidy的源代码,增加一个C++的功能检查.希望检查每一个类/struct/union中,如果有类型位整形的位,这个位的长度必须大于1

void BitFieldWidthCheck::registerMatchers(MatchFinder *Finder) {
  // Match bit-fields that are of integer types
   Finder->addMatcher(  
      fieldDecl(
		isBitField(),
		anyOf(
			hasType(asString("int")),
			hasType(asString("short"))
		)
	)		
	.bind("integerBitField"), this  );  
}
	  
void BitFieldWidthCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *BitField = Result.Nodes.getNodeAs<FieldDecl>("integerBitField");
  //const auto *Width = Result.Nodes.getNodeAs<IntegerLiteral>("width");
  
  if (!BitField)
    return;

// Access the bit-width expression
  const Expr *WidthExpr = BitField->getBitWidth();
  if (!WidthExpr)
    return;
  
  // Evaluate the bit width
  Expr::EvalResult EvalResult;
  if (!WidthExpr->EvaluateAsInt(EvalResult, *Result.Context))
    return;
  
  llvm::APSInt WidthValue = EvalResult.Val.getInt();
  unsigned uWidth = WidthValue.getZExtValue();


  if (uWidth <= 1) {  
    // 示例：输出匹配到的位域名称和位置  
    diag(BitField->getLocation(), "违反GJB R-1-1-10 Found integer bit-field: %0")  
        << BitField->getName();  
  }  
  
  
}

} // namespace dkaq
} // namespace tidy 
} // namespace clang 