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

CSS::CSSStyleDeclaration* CSSParser::consumeAListOfDeclarations() {
  ToyScopyUtil::logUtil("## start consume a list of declarations");
  CSS::CSSStyleDeclaration* block = new CSS::CSSStyleDeclaration();
  while (tokenizer->pumpToken()) {
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
  ToyScopyUtil::logUtil("## start consume a simple block");
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
  while (tokenizer->pumpToken()) {
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
      CSSToken* value = consumeAComponentValue();
      ToyScopyUtil::logUtil("** token value: >%s<", value->getValue().c_str());
      ToyScopyUtil::logUtil("** token type: %d", value->getTokenType());
      // TODO: append this to the value of the block
    }
  }
  return block;
}

CSSToken* CSSParser::consumeAComponentValue() {
  ToyScopyUtil::logUtil("## start consume a component value");
  if (!tokenizer->canTakeNextToken()) {
    return nullptr;
  }
  ToyScopyUtil::logUtil("tokenizer next token");
  CSSToken* token = tokenizer->nextToken();
  ToyScopyUtil::logUtil("consumetoken");
  tokenizer->consumeToken();
  ToyScopyUtil::logUtil("gettokentype");
  CSSToken::CSSTokenType tokenType = token->getTokenType();
  ToyScopyUtil::logUtil("type: %d", tokenType);

  if (tokenType == CSSToken::LeftCurlyBracketToken ||
      tokenType == CSSToken::LeftBlockBracketToken ||
      tokenType == CSSToken::LeftBracketToken) {
    // consumeASimpleBlock and return it
    CSS::CSSStyleDeclaration* dec = consumeASimpleBlock();
  } else if (tokenType == CSSToken::FunctionToken) {
    // consume a function and return it
  } else {
    ToyScopyUtil::logUtil("returned component value > type: %d: value: %s",
                          token->getTokenType(), token->getValue().c_str());
    return token;
  }
  return token;
}

CSS::CSSStyleRule* CSSParser::consumeAQualifiedRule() {
  ToyScopyUtil::logUtil("## start consume a qualified rule");
  CSS::CSSStyleRule* cssStyleRule = new CSS::CSSStyleRule();

  // it depends on the context
  unsigned short prelude = CSS::CSSRule::STYLE_RULE;

  while (tokenizer->pumpToken()) {
    CSSToken* token = tokenizer->nextToken();

    tokenizer->consumeToken();
    CSSToken::CSSTokenType tokenType = token->getTokenType();
    ToyScopyUtil::logUtil("next consumeToken>> %d", tokenType);
    if (tokenType == CSSToken::EOFToken) {
      // This is a parser error. return nothing.
      return nullptr;
    } else if (tokenType == CSSToken::LeftCurlyBracketToken) {
      // consume a simple block and assign it to the qualified rule's block
      // return the qualified rule
      setCurrentEndingToken(token);
      CSS::CSSStyleDeclaration* declaration = consumeASimpleBlock();
      cssStyleRule->appendDeclarations(declaration);
      return cssStyleRule;
    } else if (isSimpleBlockWithAnAssociatedToken()) {
    } else {
      tokenizer->reconsumeToken();
      CSSToken* selectorToken = consumeAComponentValue();
      // append the rules prelude
      cssStyleRule->setSelectorText(selectorToken->getValue());
    }
  }
  return cssStyleRule;
}

CSS::CSSRuleList* CSSParser::consumeAListOfRule() {
  ToyScopyUtil::logUtil("## start consume a list of rule");
  CSS::CSSRuleList* cssRuleList = new CSS::CSSRuleList();

  while (tokenizer->pumpToken()) {
    CSSToken* token = tokenizer->nextToken();
    ToyScopyUtil::logUtil("token:: %s", token->getValue().c_str());
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
      // TODO: Consume an at rule
    } else {
      // re-consume the current input token
      tokenizer->reconsumeToken();
      consumeAQualifiedRule();
      // consume a component value
    }
  }
  return nullptr;
}

// parse a stylesheet
CSS::CSSStyleSheet* CSSParser::parseAStyleSheet() {
  ToyScopyUtil::logUtil("## start parse a style sheet");
  CSS::CSSStyleSheet* sheet = new CSS::CSSStyleSheet();
  // Consume a list of rule
  // TODO: set top level flag
  CSS::CSSRuleList* cssRuleList = consumeAListOfRule();
  sheet->setCSSRuleList(cssRuleList);
  // listRule を sheet に付加する
  return sheet;
}

CSS::CSSStyleSheet* CSSParser::parse() {
  ToyScopyUtil::logUtil("## parse");

  CSS::CSSStyleSheet* sheet = parseAStyleSheet();

  ToyScopyUtil::logUtil("## parse completed");

  return nullptr;
}
}  // namespace Flash