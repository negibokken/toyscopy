#include "html_document_parser.h"

void HTMLDocumentParser::parse() {
  while (tokenizer->pumpToken()) {
    if (!tokenizer->canTakeNextToken()) continue;

    Tag::Token* token = tokenizer->nextToken();

    if (token->type == Tag::Token::Type::StartTag ||
        token->type == Tag::Token::Type::EndTag) {
      std::cout << "tagname: " << token->tagName << std::endl;
    } else if (token->type == Tag::Token::Type::Character) {
      std::cout << "char: " << token->value << std::endl;
    }

    // Build DOM Tree
    switch (this->insertion_mode) {
      case Mode::initial: {
        if (isToken(Tag::Token::Type::DOCTYPE)) {
          DOM::Node* doctype =
              this->document->implementation->createDocumentType(
                  token->getTagName(), "", "");
          document->appendChild(doctype);
          setInsertionMode(Mode::before_html);
          tokenizer->consumeToken();
        } else {
          setInsertionMode(Mode::before_html);
        }
        break;
      }
      case Mode::before_html: {
        if (isToken(Tag::Token::Type::StartTag,
                    Tag::Token::ElementType::html)) {
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
        if (isToken(Tag::Token::Type::StartTag,
                    Tag::Token::ElementType::head)) {
          DOM::Node* n = this->document->createElement("head");
          head_pointer = n;
          setInsertionMode(Mode::in_head);
          tokenizer->consumeToken();
        }
        break;
      }
      case Mode::in_head: {
        std::cout << "3" << std::endl;
        std::cout << "============" << std::endl;
        if (isToken(Tag::Token::Character)) {
          // FIXME:
          tokenizer->consumeToken();
        } else if (isToken(Tag::Token::Type::StartTag,
                           Tag::Token::ElementType::title)) {
          DOM::Node* n = this->document->createElement("title");
          pushOpenElement(n);
          head_pointer->appendChild(n);
          setOriginalInsertionMode(Mode::in_head);
          setInsertionMode(Mode::text);
          tokenizer->consumeToken();
        } else if (isToken(Tag::Token::Type::StartTag,
                           Tag::Token::ElementType::meta)) {
          std::cout << "============" << std::endl;
          std::cout << "start tag meta tag" << std::endl;
          DOM::Node* n = this->document->createElement("meta");

          DOM::Element* ele = static_cast<DOM::Element*>(n);
          // Configure attributes
          for (auto attr : token->getAttributes()) {
            ele->setAttribute(attr->getName(), attr->getValue());
          }

          head_pointer->appendChild(ele);

          // charset settings
          if (token->hasAttribute("charset")) {
            std::string chset = token->getAttributeValue("charset");
            document->charset = chset;
            this->charset = chset;
          }

          // content-type
          if (token->hasAttribute("http-equiv") &&
              (ToyScopyUtil::toASCIIlower(
                   token->getAttributeValue("http-equiv")) == "content-type") &&
              token->hasAttribute("content")) {
            std::string ct = token->getAttributeValue("content");
            document->contentType = ct;
            this->contentType = ct;
          }
          tokenizer->consumeToken();
        } else if (isToken(Tag::Token::Type::StartTag,
                           Tag::Token::ElementType::style)) {
          std::cout << "style" << std::endl;
          // generic raw text element parsing algorihtm
          DOM::Node* n = this->document->createElement("style");
          head_pointer->appendChild(n);
          pushOpenElement(n);
          tokenizer->setState(Tokenizer::Tokenizer::RAWTEXTState);
          setOriginalInsertionMode(Mode::in_head);
          setInsertionMode(Mode::text);
          tokenizer->consumeToken();
        } else if (isToken(Tag::Token::Type::EndTag,
                           Tag::Token::ElementType::title)) {
          std::cout << "pop title-> " << std::endl;
          popOpenElementIf("title");
          tokenizer->consumeToken();
        } else if (isToken(Tag::Token::Type::EndTag,
                           Tag::Token::ElementType::head)) {
          std::cout << "go to after head ->" << std::endl;
          setInsertionMode(Mode::after_head);
          tokenizer->consumeToken();
        }
        break;
      }
      case Mode::text: {
        if (isToken(Tag::Token::Type::Character)) {
          appendCharacterToken(token->value);
          tokenizer->consumeToken();
        } else if (isToken(Tag::Token::Type::EndTag,
                           Tag::Token::ElementType::script)) {
        } else if (isToken(Tag::Token::Type::EndTag,
                           Tag::Token::ElementType::title)) {
          DOM::Node* n = findTextNode();
          DOM::Text* textNode = (static_cast<DOM::Text*>(n));
          // Title Node
          popOpenElementIf(DOM::NodeType::TEXT_NODE);
          // Title Node
          popOpenElementIf("title");
          setDocumentTitle(textNode->wholeText());
          setInsertionMode(original_insertion_mode);
          tokenizer->consumeToken();
        } else if (isToken(Tag::Token::Type::EndTag)) {
          // Text Node
          popOpenElementIf(DOM::NodeType::TEXT_NODE);
          // Some Element
          popOpenElement();
          setInsertionMode(original_insertion_mode);
          tokenizer->consumeToken();
        }
        break;
      }
      case Mode::after_head: {
        std::cout << "4" << std::endl;
        if (isToken(Tag::Token::Type::StartTag,
                    Tag::Token::ElementType::body)) {
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
        if (isToken(Tag::Token::Type::Character)) {
          appendCharacterToken(token->value);
          tokenizer->consumeToken();
        } else if (isToken(Tag::Token::Type::StartTag)) {
          DOM::Node* n = document->createElement(token->getTagName());
          appendAttributesToCurrentNode(n);
          appendToCurrentNode(n);
          pushOpenElement(n);
          tokenizer->consumeToken();
        } else if (isToken(Tag::Token::Type::EndTag,
                           Tag::Token::ElementType::body)) {
          popOpenElementIf("title");
          setInsertionMode(Mode::after_body);
          tokenizer->consumeToken();
        } else if (isToken(Tag::Token::Type::EndTag)) {
          std::cout << "***EndTag***" << std::endl;
          popOpenElementIf(DOM::NodeType::TEXT_NODE);
          popOpenElement();
          tokenizer->consumeToken();
        }
        break;
      }
      case Mode::after_body: {
        std::cout << "6" << std::endl;
        if (isToken(Tag::Token::Type::EndTag, Tag::Token::ElementType::html)) {
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
  DOM::Element* ele = static_cast<DOM::Element*>(n);
  std::cout << "*** pushed: " << ele->tagName << std::endl;
  open_elements.push_back(n);
}

void HTMLDocumentParser::popOpenElement() {
  DOM::Element* ele = static_cast<DOM::Element*>(open_elements.back());
  std::cout << "*** popped: " << ele->tagName << std::endl;
  open_elements.pop_back();
}

void HTMLDocumentParser::popOpenElementIf(std::string tagName) {
  DOM::Element* ele = static_cast<DOM::Element*>(open_elements.back());
  if (open_elements.size() == 0) return;
  if (ele->getTagName() != tagName) return;
  std::cout << "*** popped: " << ele->tagName << std::endl;
  open_elements.pop_back();
}

void HTMLDocumentParser::popOpenElementIf(DOM::NodeType type) {
  if (open_elements.size() == 0) return;
  if (open_elements.back()->nodeType != type) return;
  DOM::Text* text = static_cast<DOM::Text*>(open_elements.back());
  std::cout << "*** popped: " << text->data << std::endl;
  open_elements.pop_back();
}

void HTMLDocumentParser::setFramesetOkFlag(std::string str) {
  frameset_ok = str;
}

void HTMLDocumentParser::appendToCurrentNode(DOM::Node* n) {
  DOM::Element* ele = static_cast<DOM::Element*>(open_elements.back());
  DOM::Element* child = static_cast<DOM::Element*>(n);
  std::cout << "append to " << ele->tagName << std::endl;
  std::cout << "appended: " << child->tagName << std::endl;
  if (open_elements.size() > 0 &&
      open_elements.back()->nodeType == DOM::NodeType::TEXT_NODE) {
    popOpenElement();
  }
  std::cout << "open last: " << open_elements.back()->nodeType << std::endl;
  open_elements.back()->appendChild(n);
}

bool HTMLDocumentParser::isToken(Tag::Token::Type type,
                                 Tag::Token::ElementType eleType) {
  Tag::Token* token = tokenizer->nextToken();
  return token->type == type && token->elementType == eleType;
}

bool HTMLDocumentParser::isToken(Tag::Token::ElementType type) {
  return tokenizer->nextToken()->elementType == type;
}

bool HTMLDocumentParser::isToken(Tag::Token::Type type) {
  return tokenizer->nextToken()->type == type;
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
    std::cout << "**********" << std::endl;
    DOM::Element* ele = static_cast<DOM::Element*>(open_elements.back());
    std::cout << ele->nodeName << std::endl;
    std::cout << ele->nodeType << std::endl;
    std::cout << ele->tagName << std::endl;
    open_elements.back()->appendChild(node);
    pushOpenElement(node);
  }
  DOM::Text* textNode = (static_cast<DOM::Text*>(node));
  textNode->appendData(data);
  std::cout << "text: " << textNode->data << std::endl;
}

void HTMLDocumentParser::setOriginalInsertionMode(Mode mode) {
  original_insertion_mode = mode;
}

void HTMLDocumentParser::appendAttributesToCurrentNode(DOM::Node* n) {
  DOM::Element* ele = static_cast<DOM::Element*>(n);

  Tag::Token* token = tokenizer->nextToken();
  for (auto a : token->getAttributes()) {
    std::cout << "attr: " << a->getName() << ":" << a->getValue() << std::endl;
    ele->setAttribute(a->getName(), a->getValue());
  }
}

void HTMLDocumentParser::stopParsing() {}
