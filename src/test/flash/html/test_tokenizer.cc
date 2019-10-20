#include "../../../flash/html/tokenizer.h"
#include "gtest/gtest.h"

TEST(TokenizerTest, TokenizeSimple)
{
  Tokenizer::Tokenizer *t =
      new Tokenizer::Tokenizer("<html><body>hello</body></html>");

  char cc;
  while (cc != '\0') {
    t->nextToken();
  }
  std::cout << t->state << std::endl;
}

