#include "../../../flash/css/css_parser.h"
#include "gtest/gtest.h"

TEST(CSSParserTest, Simple) {
  const std::string src = " span {color:#ff0000; }";
  Flash::CSSParser* parser = new Flash::CSSParser(src);

  parser->parse();
}