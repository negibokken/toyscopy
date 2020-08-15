#include "../../../flash/css/css_parser.h"
#include "gtest/gtest.h"

TEST(CSSParserTest, Simple) {
  const std::string src = " span {color:#ff0000; }";
  Flash::CSSParser* parser = new Flash::CSSParser(src);

  parser->parse();

  // const std::vector<std::pair<Flash::CSSToken::CSSTokenType, std::string>>
  //    expected = {
  //        {Flash::CSSToken::WhitespaceToken, " "},
  //        {Flash::CSSToken::IdentToken, "span"},
  //        {Flash::CSSToken::WhitespaceToken, " "},
  //        {Flash::CSSToken::LeftCurlyBracketToken, "{"},
  //        {Flash::CSSToken::IdentToken, "color"},
  //        {Flash::CSSToken::ColonToken, ":"},
  //        {Flash::CSSToken::HashToken, "#ff0000"},
  //        {Flash::CSSToken::SemicolonToken, ";"},
  //        {Flash::CSSToken::WhitespaceToken, " "},
  //        {Flash::CSSToken::RightCurlyBracketToken, "}"},
  //        {Flash::CSSToken::EOFToken, "\0"},
  //    };
}