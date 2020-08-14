#include "../../../flash/css/css_tokenizer.h"
#include "gtest/gtest.h"

#include <utility>

TEST(CSSTokenizerTest, ParseSimpleText) {
  Flash::CSSTokenizer* t = new Flash::CSSTokenizer(" 'hello'");
  const std::pair<Flash::CSSToken::CSSTokenType, std::string> expected[] = {
      {Flash::CSSToken::WhitespaceToken, " "},
      {Flash::CSSToken::StringToken, "hello"}};

  // parse all
  while (t->hasNextCharacter())
    t->pumpToken();

  int cnt = 0;
  while (t->canTakeNextToken()) {
    Flash::CSSToken* token = t->nextToken();
    EXPECT_EQ(token->getTokenType(), expected[cnt].first);
    EXPECT_STREQ(token->getValue().c_str(), expected[cnt].second.c_str());
    cnt++;
  }
}

TEST(CSSTokenizerTest, ParseSimpleStyle) {
  Flash::CSSTokenizer* t = new Flash::CSSTokenizer(" span {color:#ff0000; }");
  const std::vector<std::pair<Flash::CSSToken::CSSTokenType, std::string>>
      expected = {
          {Flash::CSSToken::WhitespaceToken, " "},
          {Flash::CSSToken::IdentToken, "span"},
          {Flash::CSSToken::WhitespaceToken, " "},
          {Flash::CSSToken::LeftCurlyBracketToken, "{"},
          {Flash::CSSToken::IdentToken, "color"},
          {Flash::CSSToken::ColonToken, ":"},
          {Flash::CSSToken::HashToken, "#ff0000"},
          {Flash::CSSToken::SemicolonToken, ";"},
          {Flash::CSSToken::WhitespaceToken, " "},
          {Flash::CSSToken::RightCurlyBracketToken, "}"},
      };

  // parse all
  while (t->hasNextCharacter())
    t->pumpToken();

  int cnt = 0;
  while (t->canTakeNextToken()) {
    Flash::CSSToken* token = t->nextToken();
    std::cout << "(" << token->getValue() << ", " << token->getTokenType()
              << ")" << std::endl;
    EXPECT_EQ(token->getTokenType(), expected[cnt].first);
    EXPECT_STREQ(token->getValue().c_str(), expected[cnt].second.c_str());
    cnt++;
  }
  EXPECT_EQ(cnt, expected.size());
}
