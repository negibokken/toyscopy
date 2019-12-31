#ifndef Tokenizer_h
#define Tokenizer_h

#include <iostream>
#include <queue>
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
    SelfClosingStartTagState,
    RAWTEXTState,
    RAWTEXTLessThanSignState,
    RAWTEXTEndTagOpenState,
    RAWTEXTEndTagNameState,
    RCDATAState
  };

  State state;
  std::string stream;
  long long int index;
  Tokenizer(std::string stream);
  char isNext(char c);
  char nextInputCharacter();
  bool pumpToken();
  Tag::Token *nextToken() {
    if (!canTakeNextToken()) {
      return NULL;
    }
    Tag::Token *tok = tokenQueue.front();
    return tok;
  };
  void ignoreToken(char c);
  void setState(State state);

  void emitToken() {
    std::cout << "----" << std::endl;
    std::cout << "emit: " << token->tagName << std::endl;
    token->setTagType();
    tokenQueue.push(token);
    if (token->type == Tag::Token::StartTag) {
      lastStartToken = token;
    }
  }

  void emitToken(char c) {
    createNewToken(Tag::Token::Type::Character);
    token->appendCharacter(c);
    emitToken();
  }

  std::string temporarybuffer;
  void createNewToken(Tag::Token::Type type) { token = new Tag::Token(type); }
  void appendTagName(char c) { token->appendTagName(c); }
  void createAttribute() { token->createAttribute(); }
  void appendAttributeName(char c) { token->appendAttributeName(c); }
  void appendAttributeValue(char c) { token->appendAttributeValue(c); }
  void appendCharacter(char c) { token->appendCharacter(c); }
  void appendBuffer(char c) { temporarybuffer += c; }
  void clearBuffer() { temporarybuffer.clear(); }
  void reconsumeToken() { index--; };
  void consumeToken() {
    std::cout << "----" << std::endl;
    std::cout << "consume: " << nextToken()->tagName << std::endl;
    std::cout << "type: " << Tag::Token::TagTypeName(nextToken()->getTagType())
              << std::endl;
    tokenQueue.pop();
    token = NULL;
  }
  bool canTakeNextToken() {
    bool hasNextToken = !tokenQueue.empty();
    return hasNextToken;
  };
  bool isAppropriateEndTag();

 private:
  Tag::Token *token;
  std::queue<Tag::Token *> tokenQueue;
  Tag::Token *lastStartToken;
};

}  // namespace Tokenizer
#endif
