#include "html_document_parser.h"

#include <iostream>

#include "dom.h"

void HTMLDocumentParser::parse() {
  while (tokenizer->nextToken()) {
    // std::cout << tokenizer->token->tagName << std::endl;
    if (tokenizer->isEmited()) {
      if (tokenizer->token->type == Tag::Type::StartTag ||
          tokenizer->token->type == Tag::Type::EndTag) {
        std::cout << tokenizer->token->tagName << std::endl;
      } else if (tokenizer->token->type == Tag::Type::Character) {
        std::cout << tokenizer->token->value << std::endl;
      }
      tokenizer->consumeToken();
    }
  }
}

void HTMLDocumentParser::consumeIgnoreToken() {
  // U+0009 CHARACTER TABULATION, U+000A LINE FEED (LF), U+000C FORM FEED
  // (FF), U+000D CARRIAGE RETURN (CR), or U+0020 SPACE
  while (doc[this->itr] == '\t' || doc[this->itr] == '\n' ||
         doc[this->itr] == '\f' || doc[this->itr] == '\r' ||
         // doc[this->itr] == ' ') {
         doc[this->itr] == '\f') {
    if (doc[this->itr] == '\0') break;
    this->itr++;
  }
}

void HTMLDocumentParser::consumeToken() {}
