#ifndef Tokenizer_h
#define Tokenizer_h

#include "./tag.h"

#include <iostream>
#include <string>

namespace Tokenizer {

class Tokenizer {
  public:
  enum State { Data, TagOpenState, EndTagOpenState, TagNameState, BeforeAttributeNameState };
  State state;
  std::string stream;
  long long int index;
  Tokenizer(std::string stream);
  char nextInputCharacter();
  bool nextToken();
  void ignoreToken(char c);
  void setState(State state);
  Tag *token;
};

}  // namespace Tokenizer
#endif
