#ifndef CSS_PARSER_h
#define CSS_PARSER_h
#include <string>

#include "css.h"
#include "css_tokenizer.h"

namespace Flash {

class CSSTokenizer;

class CSSParser {
 private:
  std::string src;
  CSSTokenizer* tokenizer;
  CSS::StyleSheetList* styleSheets;
  CSS::CSSStyleSheet* parseAStyleSheet();
  CSS::CSSRuleList* consumeAListOfRule();
  CSS::CSSRule* consumeAQualifiedRule();
  CSSToken* consumeAComponentValue();
  CSS::CSSStyleDeclaration* consumeASimpleBlock();
  CSS::CSSStyleDeclaration* consumeAListOfDeclarations();
  CSSToken* currentEndingToken;
  inline void setCurrentEndingToken(CSSToken* token) {
    currentEndingToken = token;
  }

 public:
  CSSParser(std::string src);
  ~CSSParser();
  CSS::CSSStyleSheet* parse();
};

}  // namespace Flash
#endif