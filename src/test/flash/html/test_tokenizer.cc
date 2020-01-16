#include "../../../flash/html/tokenizer.h"
#include "gtest/gtest.h"

TEST(TokenizerTest, Simple) {
  Tokenizer::Tokenizer *t = new Tokenizer::Tokenizer("<html>hello</html>");

  int tokenNum = 0;
  while (t->pumpToken()) {
    if (t->canTakeNextToken()) {
      t->consumeToken();
      tokenNum++;
    }
  }
  // Start Tag + 5 Character Token + End Tag Token = 7 Token
  EXPECT_EQ(7, tokenNum);
}

TEST(TokenizerTest, SimpleWithDocType) {
  Tokenizer::Tokenizer *t =
      new Tokenizer::Tokenizer("<!DOCTYPE html><html>hello</html>");

  int tokenNum = 0;
  while (t->pumpToken()) {
    if (t->canTakeNextToken()) {
      t->consumeToken();
      tokenNum++;
    }
  }
  // DocType + Start Tag + 5 Character Token + End Tag Token = 8 Token
  EXPECT_EQ(8, tokenNum);
}

TEST(TokenizerTest, SimpleWithSelfClosingTag) {
  Tokenizer::Tokenizer *t = new Tokenizer::Tokenizer("<html>hello<br/></html>");

  int tokenNum = 0;
  while (t->pumpToken()) {
    if (t->canTakeNextToken()) {
      t->consumeToken();
      tokenNum++;
    }
  }
  // Start Tag + 5 Character Token + br tag token + End Tag Token = 7 Token
  EXPECT_EQ(8, tokenNum);
}

TEST(TokenizerTest, SimpleWithWiredDocType) {
  // Note: <!DOCTYPE  html> includes two spaces in front of name and it's Html.
  Tokenizer::Tokenizer *t =
      new Tokenizer::Tokenizer("<!DOCTYPE  HTml><html>hello</html>");

  int tokenNum = 0;
  while (t->pumpToken()) {
    if (t->canTakeNextToken()) {
      t->consumeToken();
      tokenNum++;
    }
  }
  // DocType + Start Tag + 5 Character Token + End Tag Token = 8 Token
  EXPECT_EQ(8, tokenNum);
}

TEST(TokenizerTest, NoneToken) {
  Tokenizer::Tokenizer *t = new Tokenizer::Tokenizer("");
  Tag::Token *tok = t->nextToken();
  EXPECT_EQ(nullptr, tok);
}

TEST(TokenizerTest, AttributeSimpleTest) {
  Tokenizer::Tokenizer *t = new Tokenizer::Tokenizer(
      "<html><head><meta "
      " charset=\"utf-8\" />"
      "</head>hello</html>");

  int tokenNum = 0;
  while (t->pumpToken()) {
    if (t->canTakeNextToken()) {
      t->consumeToken();
      tokenNum++;
    }
  }
  // 5 Tag Token + 5 Character
  EXPECT_EQ(10, tokenNum);
}

TEST(TokenizerTest, RAWTextIncludeLessThanSignTest) {
  // Note: style tag include Capital Character
  Tokenizer::Tokenizer *t = new Tokenizer::Tokenizer(
      "<sTyLe> body { } a::after { content: \"<\"; } </sTyLe>");

  int tokenNum = 0;
  while (t->pumpToken()) {
    if (t->canTakeNextToken()) {
      t->consumeToken();
      tokenNum++;
    }
    // After <style>, set state of tokenizer to RAWText
    if (tokenNum == 1) {
      t->setState(Tokenizer::Tokenizer::RAWTEXTState);
    }
  }
  // 2 Tag Token + 37 Character in RAWTEXT
  EXPECT_EQ(39, tokenNum);
}

TEST(TokenizerTest, RAWTextIncludeLessThanSignAndSlashTest) {
  // Note: style tag include Capital Character
  Tokenizer::Tokenizer *t = new Tokenizer::Tokenizer(
      "<sTyLe> body { } a::after { content: \"</\"; } </sTyLe>");

  int tokenNum = 0;
  while (t->pumpToken()) {
    if (t->canTakeNextToken()) {
      Tag::Token *tok = t->nextToken();
      t->consumeToken();
      tokenNum++;
    }
    // After <style>, set state of tokenizer to RAWText
    if (tokenNum == 1) {
      t->setState(Tokenizer::Tokenizer::RAWTEXTState);
    }
  }
  // 2 Tag Token + 38 Character in RAWTEXT
  EXPECT_EQ(40, tokenNum);
}

TEST(TokenizerTest, SimpleWithAnchor) {
  // Note: second attribute value should be Capital
  Tokenizer::Tokenizer *t = new Tokenizer::Tokenizer(
      "<html><a HRef =\"https://example.com\" "
      "href=\"https://example.com\"><test test=\"test\"/><test "
      "test>hello</a></html>");

  int tokenNum = 0;
  while (t->pumpToken()) {
    if (t->canTakeNextToken()) {
      t->consumeToken();
      tokenNum++;
    }
  }
  // 6 Tag + 5 Character Token
  EXPECT_EQ(11, tokenNum);
}

TEST(TokenizerTest, SimpleWeirdTag) {
  Tokenizer::Tokenizer *t = new Tokenizer::Tokenizer(
      "<html><invalidtag true content= 'single_quoted' "
      "content=unquoted content=unquoted2>hello</invalid></html>");

  int tokenNum = 0;
  while (t->pumpToken()) {
    if (t->canTakeNextToken()) {
      t->consumeToken();
      tokenNum++;
    }
  }
  // Start Tag + Weird Tag + 5 Character Token + Weird End Tag + End Tag Token =
  // 9 Token
  EXPECT_EQ(9, tokenNum);
}
