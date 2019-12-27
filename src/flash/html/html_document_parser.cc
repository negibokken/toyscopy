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
        if (isToken(Tag::Type::DOCTYPE)) {
          DOM::Node* doctype =
              this->document->implementation->createDocumentType(
                  tokenizer->token->getTagName(), "", "");
          document->appendChild(doctype);
          setInsertionMode(Mode::before_html);
          tokenizer->consumeToken();
        } else {
          setInsertionMode(Mode::before_html);
        }
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
        }
        break;
      }
      case Mode::before_head: {
        std::cout << "2" << std::endl;
        if (isToken(Tag::Type::StartTag, Tag::ElementType::head)) {
          DOM::Node* n = this->document->createElement("head");
          head_pointer = n;
          setInsertionMode(Mode::in_head);
          tokenizer->consumeToken();
        }
        break;
      }
      case Mode::in_head: {
        std::cout << "3" << std::endl;
        if (isToken(Tag::Type::StartTag, Tag::ElementType::title)) {
          DOM::Node* n = this->document->createElement("title");
          pushOpenElement(n);
          head_pointer->appendChild(n);
          setOriginalInsertionMode(Mode::in_head);
          setInsertionMode(Mode::text);
          tokenizer->consumeToken();
        } else if (isToken(Tag::Type::EndTag, Tag::ElementType::title)) {
          popOpenElementIf("title");
          tokenizer->consumeToken();
        } else if (isToken(Tag::Type::EndTag, Tag::ElementType::head)) {
          setInsertionMode(Mode::after_head);
          tokenizer->consumeToken();
        }
        break;
      }
      case Mode::text: {
        if (isToken(Tag::Type::Character)) {
          appendCharacterToken(tokenizer->token->value);
          tokenizer->consumeToken();
        } else if (isToken(Tag::Type::EndTag, Tag::ElementType::script)) {
        } else if (isToken(Tag::Type::EndTag, Tag::ElementType::title)) {
          DOM::Node* n = findTextNode();
          DOM::Text* textNode = (static_cast<DOM::Text*>(n));
          setDocumentTitle(textNode->wholeText());
          setInsertionMode(original_insertion_mode);
          popOpenElement();
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
        } else if (isToken(Tag::Type::StartTag)) {
          DOM::Node* n =
              document->createElement(tokenizer->token->getTagName());
          appendAttributesToCurrentNode(n);
          appendToCurrentNode(n);
          pushOpenElement(n);
          tokenizer->consumeToken();
        } else if (isToken(Tag::Type::EndTag, Tag::ElementType::body)) {
          setInsertionMode(Mode::after_body);
          tokenizer->consumeToken();
        } else if (isToken(Tag::Type::EndTag)) {
          popOpenElementIf(tokenizer->token->getTagName());
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

void HTMLDocumentParser::setInsertionMode(Mode mode) {
  const std::string Mode[] = {"initial",          "before_html",
                              "before_head",      "in_head",
                              "in_head_noscript", "after_head",
                              "in_body",          "text",
                              "in_table",         "in_table_text",
                              "in_caption",       "in_column_group",
                              "in_table_body",    "in_row",
                              "in_cell",          "in_select",
                              "in_template",      "after_body",
                              "in_frameset",      "after_frameset",
                              "after_after_body", "after_after_frameset"};
  std::cout << "=========================" << std::endl;
  std::cout << "## mode: " << Mode[mode] << std::endl;
  insertion_mode = mode;
}

void HTMLDocumentParser::pushOpenElement(DOM::Node* n) {
  open_elements.push_back(n);
}

void HTMLDocumentParser::popOpenElement() { this->open_elements.pop_back(); }
void HTMLDocumentParser::popOpenElementIf(std::string tagName) {
  if (open_elements.size() == 0) return;
  if (static_cast<DOM::Element*>(open_elements.back())->getTagName() != tagName)
    return;
  open_elements.pop_back();
}

void HTMLDocumentParser::setFramesetOkFlag(std::string str) {
  frameset_ok = str;
}

void HTMLDocumentParser::appendToCurrentNode(DOM::Node* n) {
  if (open_elements.size() > 0 &&
      open_elements.back()->nodeType == DOM::NodeType::TEXT_NODE) {
    popOpenElement();
  }
  std::cout << "open last: " << open_elements.back()->nodeType << std::endl;
  open_elements.back()->appendChild(n);
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

bool HTMLDocumentParser::isToken(Tag::ElementType type) {
  return tokenizer->token->elementType == type;
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
  std::cout << "### append character token" << std::endl;
  DOM::Node* node = findTextNode();
  if (node == NULL) {
    std::cout << "create text node" << std::endl;
    node = document->createText("");
    const int last = open_elements.size() - 1;
    open_elements[last]->appendChild(node);
    pushOpenElement(node);
  }
  DOM::Text* textNode = (static_cast<DOM::Text*>(node));
  textNode->appendData(data);
  std::cout << "text: " << textNode->data << std::endl;
}

void HTMLDocumentParser::setOriginalInsertionMode(Mode mode) {
  original_insertion_mode = mode;
}

void HTMLDocumentParser::appendAttributesToCurrentNode(DOM::Node *n) {
  DOM::Element* ele = static_cast<DOM::Element*>(n);
  for (auto a : tokenizer->token->getAttributes()) {
    std::cout << "attr: "<<  a->getName() << ":" << a->getValue() << std::endl;
    ele->setAttribute(a->getName(), a->getValue());
  }
}

void HTMLDocumentParser::stopParsing() {}
