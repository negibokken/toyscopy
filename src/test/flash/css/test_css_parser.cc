#include "../../../flash/css/css_parser.h"
#include "gtest/gtest.h"

TEST(CSSParserTest, SimpleTextColor) {
  using namespace Flash;
  const std::string src = " span {color:#ff0000; }";
  CSSParser* parser = new Flash::CSSParser(src);
  CSS::CSSStyleSheet* sheet = parser->parse();

  ASSERT_TRUE(sheet != nullptr);

  CSS::CSSRuleList list = *sheet->getCSSRules();
  for (int i = 0; i < list.size(); i++) {
    const CSS::CSSRule* rule = list[i];
  }
}