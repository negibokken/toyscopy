#ifndef Tokenizer_h
#define Tokenizer_h

#include <iostream>
#include <string>

#include "token.h"

namespace Tokenizer {

class Tokenizer {
 private:
  bool emitted;

 public:
  enum State {
    DataState,
    TagOpenState,
    EndTagOpenState,
    TagNameState,
    BeforeAttributeNameState,
    AttributeNameState,
    AttributeValueState,
    AttributeValueUnQuotedState,
    AttributeValueSingleQuotedState,
    AttributeValueDoubleQuotedState,
    AfterAttributeNameState,
    AfterAttributeValueQuotedState,
    BeforeAttributeValueState,
    MarkdownDeclarationOpenState,
    DoctypeState,
    BeforeDoctypeNameState,
    DoctypeNameState,
    SelfClosingStartTagState
  };

  State state;
  std::string stream;
  long long int index;
  Tokenizer(std::string stream);
  char isNext(char c);
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

  Tag::Token *token;
  std::string temporarybuffer;
  void createNewToken(Tag::Token::Type type) { token = new Tag::Token(type); }
  void appendTagName(char c) { token->appendTagName(c); }
  void createAttribute() { token->createAttribute(); }
  void appendAttributeName(char c) { token->appendAttributeName(c); }
  void appendAttributeValue(char c) { token->appendAttributeValue(c); }
  void appendCharacter(char c) { token->appendCharacter(c); }
  void appendBuffer(char c) { temporarybuffer += c; }
  void clearBuffer() { temporarybuffer.clear(); }
  bool isEmitted() { return emitted; }
  void reconsumeToken() { index--; };
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
