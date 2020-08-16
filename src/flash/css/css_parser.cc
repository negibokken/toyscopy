#include "css_parser.h"

namespace Flash {

// TODO:
bool isSimpleBlockWithAnAssociatedToken() {
  return false;
}

CSSParser::CSSParser(std::string src) : src(src) {
  tokenizer = new CSSTokenizer(src);
}

CSSParser::~CSSParser() {
  delete tokenizer;
}

CSSToken* CSSParser::consumeASimpleBlock() {
  // aaa
  CSSToken* token;
  return token;
}

CSSToken* CSSParser::consumeAComponentValue() {
  if (!tokenizer->canTakeNextToken()) {
    return nullptr;
  }
  CSSToken* token = tokenizer->nextToken();
  tokenizer->consumeToken();
  CSSToken::CSSTokenType tokenType = token->getTokenType();

  if (tokenType == CSSToken::LeftCurlyBracketToken ||
      tokenType == CSSToken::LeftBlockBracketToken ||
      tokenType == CSSToken::LeftBracketToken) {
    // consumeASimpleBlock and return it
  } else if (tokenType == CSSToken::FunctionToken) {
    // consume a function and return it
  } else {
    return token;
  }
  return nullptr;
}

CSS::CSSRule* CSSParser::consumeAQualifiedRule() {
  CSS::CSSRuleList* cssRuleList = new CSS::CSSRuleList();

  while (tokenizer->canTakeNextToken()) {
    CSSToken* token = tokenizer->nextToken();
    tokenizer->consumeToken();
    CSSToken::CSSTokenType tokenType = token->getTokenType();
    if (tokenType == CSSToken::EOFToken) {
      // This is a parser error. return nothing.
      return nullptr;
    } else if (tokenType == CSSToken::LeftCurlyBracketToken) {
      // consume a simple block and assign it to the qualified rule's block
      // return the qualified rule
      setCurrentEndingToken(token);
      consumeASimpleBlock();
    } else if (isSimpleBlockWithAnAssociatedToken()) {
    } else {
      tokenizer->reconsumeToken();
      auto Something = consumeAComponentValue();
      // append
    }
  }
}

CSS::CSSRuleList* CSSParser::consumeAListOfRule() {
  CSS::CSSRuleList* cssRuleList = new CSS::CSSRuleList();
  while (tokenizer->canTakeNextToken()) {
    CSSToken* token = tokenizer->nextToken();
    tokenizer->consumeToken();
    CSSToken::CSSTokenType tokenType = token->getTokenType();
    if (tokenType == CSSToken::WhitespaceToken) {
      // Do nothing
    } else if (tokenType == CSSToken::EOFToken) {
      return cssRuleList;
    } else if (tokenType == CSSToken::CDOToken ||
               tokenType == CSSToken::CDCToken) {
      // TODO:
    } else if (tokenType == CSSToken::AtKeywordToken) {
    } else {
      // reconsume the current input token
      tokenizer->reconsumeToken();
      consumeAQualifiedRule();
      // consume a component value
    }
    return nullptr;
  }
}

void CSSParser::parse() {
  while (tokenizer->pumpToken())
    ;

  // while (tokenizer->canTakeNextToken()) {
  //   CSSToken* token = tokenizer->nextToken();
  //   tokenizer->consumeToken();
  //   // ## parser stylesheet
  //   // ### consume a list of a rule
  //   if (token->getTokenType() == CSSToken::WhitespaceToken) {
  //     // do nothing
  //   }
  // }
}
}  // namespace Flash