#ifndef Tokenizer_h
#define Tokenizer_h

#include <iostream>
#include <string>

#include "./tag.h"

namespace Tokenizer {

class Tokenizer {
 private:
  bool emitted;

 public:
  enum State {
    Data,
    TagOpenState,
    EndTagOpenState,
    TagNameState,
    BeforeAttributeNameState,
    MarkdownDeclarationOpenState,
    DoctypeState,
    BeforeDoctypeNameState,
    DoctypeNameState
  };
  State state;
  std::string stream;
  long long int index;
  Tokenizer(std::string stream);
  char nextInputCharacter();
  bool nextToken();
  void ignoreToken(char c);
  void setState(State state);
  void emitToken() {
    std::cout << "----" << std::endl;
    std::cout << "emit: " << token->tagName << std::endl;
    token->setTagType();
    emitted = true;
  }

  Tag *token;
  std::string temporarybuffer;
  void createNewToken(Tag::Type type) { token = new Tag(type); }
  void appendTagName(char c) { token->appendTagName(c); }
  void appendCharacter(char c) { token->appendCharacter(c); }
  void appendBuffer(char c) { temporarybuffer += c; }
  void clearBuffer() { temporarybuffer.clear(); }
  bool isEmitted() { return emitted; }
  void consumeToken() {
    std::cout << "----" << std::endl;
    std::cout << "consume: " << token->tagName << std::endl;
    emitted = false;
    token = NULL;
  }
  bool canTakeNextToken() {
    bool hasNextToken = nextToken();
    return hasNextToken;
  };
};

}  // namespace Tokenizer
#endif
