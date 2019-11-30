#ifndef Tokenizer_h
#define Tokenizer_h

#include <iostream>
#include <string>

#include "./tag.h"

namespace Tokenizer {

class Tokenizer {
 private:
  bool emited;

 public:
  enum State {
    Data,
    TagOpenState,
    EndTagOpenState,
    TagNameState,
    BeforeAttributeNameState
  };
  State state;
  std::string stream;
  long long int index;
  Tokenizer(std::string stream);
  char nextInputCharacter();
  bool nextToken();
  void ignoreToken(char c);
  void setState(State state);
  void emitToken() { emited = true; }

  Tag *token;
  std::string temporarybuffer;
  void createNewToken(Tag::Type type) { token = new Tag(type); }
  void appendTagName(char c) { token->appendTagName(c); }
  void appendCharacter(char c) { token->appendCharacter(c); }
  void appendBuffer(char c) { temporarybuffer += c; }
  void clearBuffer() { temporarybuffer.clear(); }
  bool isEmited() { return emited; }
  void consumeToken() { emited = false; }
};

}  // namespace Tokenizer
#endif
