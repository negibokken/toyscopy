#include "../../../flash/html/tokenizer.h"
#include "gtest/gtest.h"

TEST(TokenizerTest, TokenizeSimple) {
  Tokenizer::Tokenizer *t =
      new Tokenizer::Tokenizer("<html><body>hello</body></html>");

  char cc;
  while (t->nextToken()) {
  }
}

