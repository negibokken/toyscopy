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
  CSS::CSSRuleList* consumeAListOfRule();
  CSS::CSSRule* consumeAQualifiedRule();
  CSSToken* consumeAComponentValue();
  CSSToken* consumeASimpleBlock();
  CSSToken* currentEndingToken;
  inline void setCurrentEndingToken(CSSToken* token) {
    currentEndingToken = token;
  }

 public:
  CSSParser(std::string src);
  ~CSSParser();
  void parse();
};

}  // namespace Flash