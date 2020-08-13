#include "../../../flash/css/css_tokenizer.h"
#include "gtest/gtest.h"

#include <utility>

TEST(CSSTokenizerTest, ParseSimpleText) {
  Flash::CSSTokenizer* t = new Flash::CSSTokenizer(" 'hello'");
  const std::pair<Flash::CSSToken::CSSTokenType, std::string> expected[] = {
      {Flash::CSSToken::WhitespaceToken, " "},
      {Flash::CSSToken::StringToken, "hello"}};

  // parse all
  while (t->canTakeNextToken())
    t->pumpToken();

  int cnt = 0;
  while (!t->isEmpty()) {
    Flash::CSSToken* token = t->nextToken();
    EXPECT_EQ(token->getTokenType(), expected[cnt].first);
    EXPECT_STREQ(token->getValue().c_str(), expected[cnt].second.c_str());
    cnt++;
  }
}
