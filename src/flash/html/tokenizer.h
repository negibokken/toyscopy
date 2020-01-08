#ifndef Tokenizer_h
#define Tokenizer_h

#include <iostream>
#include <queue>
#include <string>

#include "../../utils/log_util.h"
#include "token.h"
#include "util.h"

namespace Tokenizer {

class Tokenizer {
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

  Tokenizer(std::string stream);
  bool pumpToken();
  Tag::Token *nextToken() {
    if (!canTakeNextToken()) {
      return NULL;
    }
    Tag::Token *tok = tokenQueue.front();
    return tok;
  };
  void setState(State state);

  bool canTakeNextToken() {
    bool hasNextToken = !tokenQueue.empty();
    return hasNextToken;
  };
  void consumeToken() {
    ToyScopyUtil::logUtil("--- consume ---");
    ToyScopyUtil::logUtil(
        "type: %c", Tag::Token::TagTypeName(nextToken()->getTagType()).c_str());
    tokenQueue.pop();
    token = NULL;
  }

 private:
  State state;
  bool emitted;
  std::string stream;
  long long int index;
  Tag::Token *token;
  std::queue<Tag::Token *> tokenQueue;
  Tag::Token *lastStartToken;

  char isNext(char c);
  char nextInputCharacter();
  bool isAppropriateEndTag();
  void ignoreToken(char c);
  std::string temporarybuffer;
  void createNewToken(Tag::Token::Type type) { token = new Tag::Token(type); }
  void appendTagName(char c) {
    c = ToyScopyUtil::asciiUpper2lower(c);
    token->appendTagName(c);
  }
  void createAttribute() { token->createAttribute(); }
  void appendAttributeName(char c) { token->appendAttributeName(c); }
  void appendAttributeValue(char c) { token->appendAttributeValue(c); }
  void appendCharacter(char c) { token->appendCharacter(c); }
  void appendBuffer(char c) { temporarybuffer += c; }
  void clearBuffer() { temporarybuffer.clear(); }
  void reconsumeToken() { index--; };

  void emitToken() {
    ToyScopyUtil::logUtil("----");
    ToyScopyUtil::logUtil("emit: %s", token->getTagName().c_str());
    // token->setTagType();
    tokenQueue.push(token);
    if (token->getTagType() == Tag::Token::StartTag) {
      lastStartToken = token;
    }
  }

  void emitToken(char c) {
    createNewToken(Tag::Token::Type::Character);
    token->appendCharacter(c);
    emitToken();
  }
};

}  // namespace Tokenizer
#endif
