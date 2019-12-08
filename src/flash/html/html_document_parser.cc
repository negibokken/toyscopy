#include "html_document_parser.h"

#include <iostream>

#include "dom.h"

void HTMLDocumentParser::parse() {
  while (tokenizer->canTakeNextToken()) {
    if (!tokenizer->isEmitted()) {
      continue;
    }

    if (tokenizer->token->type == Tag::Type::StartTag ||
        tokenizer->token->type == Tag::Type::EndTag) {
      std::cout << "tagname: " << tokenizer->token->tagName << std::endl;
    } else if (tokenizer->token->type == Tag::Type::Character) {
      std::cout << "char: " << tokenizer->token->value << std::endl;
    }

    // Build DOM Tree
    switch (this->insertion_mode) {
      case Mode::initial: {
        setInsertionMode(Mode::before_html);
        break;
      }
      case Mode::before_html: {
        if (isToken(Tag::Type::StartTag, Tag::ElementType::html)) {
          std::cout << "1" << std::endl;
          DOM::Node* n = this->document->createElement("html");
          document->appendChild(n);
          pushOpenElement(n);

          setInsertionMode(Mode::before_head);
          tokenizer->consumeToken();
          break;
        }
      }
      case Mode::before_head: {
        std::cout << "2" << std::endl;
        if (isToken(Tag::Type::StartTag, Tag::ElementType::head)) {
          std::cout << "**************" << std::endl;
          DOM::Node* n = this->document->createElement("head");
          head_pointer = n;
          setInsertionMode(Mode::in_head);
          tokenizer->consumeToken();
        }
        break;
      }
      case Mode::in_head: {
        std::cout << "3" << std::endl;
        if (isToken(Tag::Type::EndTag, Tag::ElementType::head)) {
          setInsertionMode(Mode::after_head);
          tokenizer->consumeToken();
        }
        break;
      }
      case Mode::after_head: {
        std::cout << "4" << std::endl;
        if (isToken(Tag::Type::StartTag, Tag::ElementType::body)) {
          DOM::Node* n = document->createElement("body");
          appendToCurrentNode(n);
          pushOpenElement(n);

          setFramesetOkFlag("not ok");

          setInsertionMode(Mode::in_body);
          tokenizer->consumeToken();
        }
        break;
      }
      case Mode::in_body: {
        std::cout << "5" << std::endl;
        if (isToken(Tag::Type::Character)) {
          appendCharacterToken(tokenizer->token->value);
          tokenizer->consumeToken();
        } else if (isToken(Tag::Type::EndTag, Tag::ElementType::body)) {
          setInsertionMode(Mode::after_body);
          tokenizer->consumeToken();
        }
        break;
      }
      case Mode::after_body: {
        std::cout << "6" << std::endl;
        if (isToken(Tag::Type::EndTag, Tag::ElementType::html)) {
          setInsertionMode(Mode::after_after_body);
          tokenizer->consumeToken();
        }
        break;
      }
      case Mode::after_after_body: {
        std::cout << "7" << std::endl;
        this->stopParsing();
        return;
      }
      default: {
        tokenizer->consumeToken();
        break;
      }
    }
  }
}

void HTMLDocumentParser::setInsertionMode(Mode mode) { insertion_mode = mode; }

void HTMLDocumentParser::pushOpenElement(DOM::Node* n) {
  open_elements.push_back(n);
}

void HTMLDocumentParser::popOpenElement() { this->open_elements.pop_back(); }

void HTMLDocumentParser::setFramesetOkFlag(std::string str) {
  frameset_ok = str;
}

void HTMLDocumentParser::appendToCurrentNode(DOM::Node* n) {
  open_elements[0]->appendChild(n);
}

bool HTMLDocumentParser::isToken(Tag::Type type, Tag::ElementType eleType) {
  std::cout << "isToken: " << tokenizer->token->type << ":" << type << ", "
            << tokenizer->token->elementType << ":";
  std::cout << (tokenizer->token->type == type &&
                tokenizer->token->elementType == eleType)
            << std::endl;

  return tokenizer->token->type == type &&
         tokenizer->token->elementType == eleType;
}

bool HTMLDocumentParser::isToken(Tag::Type type) {
  return tokenizer->token->type == type;
}

DOM::Node* HTMLDocumentParser::findTextNode() {
  for (int i = open_elements.size() - 1; i >= 0; i--) {
    if (open_elements[i]->nodeType == DOM::NodeType::TEXT_NODE) {
      return open_elements[i];
    }
  }
  return NULL;
}

void HTMLDocumentParser::appendCharacterToken(std::string data) {
  std::cout << "==== append character token ======" << std::endl;
  DOM::Node* node = findTextNode();
  if (node == NULL) {
    std::cout << "create text node" << std::endl;
    node = document->createText("");
    open_elements[0]->appendChild(node);
    pushOpenElement(node);
  }
  DOM::Text* textNode = (static_cast<DOM::Text*>(node));
  textNode->appendData(data);
  std::cout << "text: " << textNode->data << std::endl;
}

void HTMLDocumentParser::stopParsing() {}
