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

	unsigned BitWidth = BitField->getBitWidthValue(Result.Context);

const Expr *WidthExpr = BitField->getBitWidth();
	if (!WidthExpr)
    return;


	if (BitField) {  
    // 示例：输出匹配到的位域名称和位置  
    diag(BitField->getLocation(), "违反GJB R-1-1-10 Found integer bit-field: %0")  
        << BitField->getName()  
        << BitField->getSourceRange();  
  }  
  
  
}

} // namespace dkaq
} // namespace tidy 
} // namespace clang 