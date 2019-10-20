#ifndef Tokenizer_h
#define Tokenizer_h

#include <iostream>
#include <string>

namespace Tokenizer {

class Tokenizer {
  public:
  enum State { Data, TagOpenState, EndTagOpenState, TagNameState };
  State state;
  std::string stream;
  long long int index;
  Tokenizer(std::string stream);
  char nextInputCharacter();
  void nextToken();
  void setState(State state);
};

}  // namespace Tokenizer
#endif
