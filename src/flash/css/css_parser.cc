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

// CSS::CSSStyleDeclaration* CSSParser::consumeAListOfDeclarations() {
CSS::CSSStyleDeclaration* CSSParser::consumeAListOfDeclarations() {
  CSS::CSSStyleDeclaration* block = new CSS::CSSStyleDeclaration();
  while (tokenizer->canTakeNextToken()) {
    auto token = tokenizer->nextToken();
    tokenizer->consumeToken();
    auto tokenType = token->getTokenType();
    if (tokenType == CSSToken::WhitespaceToken ||
        tokenType == CSSToken::SemicolonToken) {
      // Do nothing
    } else if (tokenType == CSSToken::EOFToken) {
      return block;
    } else if (tokenType == CSSToken::AtKeywordToken) {
      // TODO:
    } else if (tokenType == CSSToken::IdentToken) {
      //
    } else {
      // this is a parse error
    }
  }
  return block;
}

CSS::CSSStyleDeclaration* CSSParser::consumeASimpleBlock() {
  // Determin Ending token
  CSSToken::CSSTokenType endingToken;
  if (currentEndingToken->getTokenType() == CSSToken::LeftBlockBracketToken) {
    endingToken = CSSToken::RightBlockBracketToken;
  } else if (currentEndingToken->getTokenType() == CSSToken::LeftBracketToken) {
    endingToken = CSSToken::RightBracketToken;
  } else if (currentEndingToken->getTokenType() ==
             CSSToken::LeftCurlyBracketToken) {
    endingToken = CSSToken::RightCurlyBracketToken;
  }

  // create a simple block
  CSS::CSSStyleDeclaration* block = new CSS::CSSStyleDeclaration();
  // empty list
  while (tokenizer->canTakeNextToken()) {
    CSSToken* token = tokenizer->nextToken();
    tokenizer->consumeToken();
    CSSToken::CSSTokenType tokenType = token->getTokenType();
    if (tokenType == endingToken) {
      return block;
    } else if (tokenType == CSSToken::EOFToken) {
      // this is a parse error.
      return block;
    } else {
      tokenizer->reconsumeToken();
      auto Something = consumeAComponentValue();
      // append this to the value of the block
    }
  }
  return nullptr;
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
  // create a new qualified rule with its prelude initially set to an empty list
  CSS::CSSRule* cssRule =
      CSS::CSSRuleFactory::createCSSRule(CSS::CSSRule::STYLE_RULE);

  // it depends on the context
  unsigned short prelude = CSS::CSSRule::STYLE_RULE;

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
      CSSToken* selectorToken = consumeAComponentValue();
      // append the rules prelude
      CSS::CSSStyleRule* styleRule = static_cast<CSS::CSSStyleRule*>(cssRule);
      styleRule->setSelectorText(selectorToken->getValue());
    }
  }
  return nullptr;
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
  }
  return nullptr;
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