#include <utility>

#include "../../../flash/css/css_tokenizer.h"
#include "gtest/gtest.h"

TEST(CSSTokenizerTest, ParseSimpleText) {
  Flash::CSSTokenizer* t = new Flash::CSSTokenizer(" 'hello'");
  const std::vector<std::pair<Flash::CSSToken::CSSTokenType, std::string>>
      expected = {
          {Flash::CSSToken::WhitespaceToken, " "},
          {Flash::CSSToken::StringToken, "hello"},
          {Flash::CSSToken::EOFToken, "\0"},
      };

  // parse all
  while (t->pumpToken())
    ;

  int cnt = 0;
  while (t->canTakeNextToken()) {
    ASSERT_TRUE(cnt < expected.size());
    Flash::CSSToken* token = t->nextToken();
    t->consumeToken();
    ASSERT_EQ(token->getTokenType(), expected[cnt].first);
    ASSERT_STREQ(token->getValue().c_str(), expected[cnt].second.c_str());
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
          {Flash::CSSToken::EOFToken, "\0"},
      };

  // parse all
  while (t->pumpToken())
    ;

  int cnt = 0;
  while (t->canTakeNextToken()) {
    // std::cout << ">>>>>>" << cnt << ", " << expected.size() << std::endl;
    ASSERT_TRUE(cnt < expected.size());
    Flash::CSSToken* token = t->nextToken();
    t->consumeToken();
    ASSERT_EQ(token->getTokenType(), expected[cnt].first);
    ASSERT_STREQ(token->getValue().c_str(), expected[cnt].second.c_str());
    cnt++;
  }
  EXPECT_EQ(cnt, expected.size());
}
