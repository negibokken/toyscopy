#include "../../../flash/html/tokenizer.h"
#include "gtest/gtest.h"

TEST(TokenizerTest, TokenizeSimple)
{
  Tokenizer::Tokenizer *t =
      new Tokenizer::Tokenizer("<html><body>hello</body></html>");

  char cc;
  // while ((cc = t->nextInputCharacter()) != '\0') {
  //   std::cout  << cc << std::endl;
  // }
  while (t->nextToken() != '\0') {
  }
  // std::cout << t->state << std::endl;
}

