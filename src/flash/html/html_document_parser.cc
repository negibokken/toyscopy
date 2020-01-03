#include "html_document_parser.h"

void HTMLDocumentParser::parse() {
  while (tokenizer->pumpToken()) {
    if (!tokenizer->canTakeNextToken()) continue;

    Tag::Token* token = tokenizer->nextToken();

    if (token->getTagType() == Tag::Token::Type::StartTag ||
        token->getTagType() == Tag::Token::Type::EndTag) {
      ToyScopyUtil::logUtil("tagname: %s", token->getTagName().c_str());
    } else if (token->getTagType() == Tag::Token::Type::Character) {
      ToyScopyUtil::logUtil("char: %s", token->getValue().c_str());
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
          ToyScopyUtil::logUtil("1");
          DOM::Node* n = this->document->createElement("html");
          document->appendChild(n);
          pushOpenElement(n);

          setInsertionMode(Mode::before_head);
          tokenizer->consumeToken();
        }
        break;
      }
      case Mode::before_head: {
        ToyScopyUtil::logUtil("2");
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
        ToyScopyUtil::logUtil("3");
        ToyScopyUtil::logUtil("=============");
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
          ToyScopyUtil::logUtil("=============");
          ToyScopyUtil::logUtil("start tag meta tag");
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
          ToyScopyUtil::logUtil("style");
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
          ToyScopyUtil::logUtil("pop title->");
          popOpenElementIf("title");
          tokenizer->consumeToken();
        } else if (isToken(Tag::Token::Type::EndTag,
                           Tag::Token::ElementType::head)) {
          ToyScopyUtil::logUtil("go to after head");
          setInsertionMode(Mode::after_head);
          tokenizer->consumeToken();
        }
        break;
      }
      case Mode::text: {
        if (isToken(Tag::Token::Type::Character)) {
          appendCharacterToken(token->getValue());
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
        ToyScopyUtil::logUtil("4");
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
        ToyScopyUtil::logUtil("5");
        if (isToken(Tag::Token::Type::Character)) {
          appendCharacterToken(token->getValue());
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
          ToyScopyUtil::logUtil("end tag");
          popOpenElementIf(DOM::NodeType::TEXT_NODE);
          popOpenElement();
          tokenizer->consumeToken();
        }
        break;
      }
      case Mode::after_body: {
        ToyScopyUtil::logUtil("6");
        if (isToken(Tag::Token::Type::EndTag, Tag::Token::ElementType::html)) {
          setInsertionMode(Mode::after_after_body);
          tokenizer->consumeToken();
        }
        break;
      }
      case Mode::after_after_body: {
        ToyScopyUtil::logUtil("7");
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

  ToyScopyUtil::logUtil("======================");
  ToyScopyUtil::logUtil("## mode: %s", Mode[mode].c_str());
  insertion_mode = mode;
}

void HTMLDocumentParser::pushOpenElement(DOM::Node* n) {
  DOM::Element* ele = static_cast<DOM::Element*>(n);
  ToyScopyUtil::logUtil("*** pushed %s", ele->tagName.c_str());
  open_elements.push_back(n);
}

void HTMLDocumentParser::popOpenElement() {
  DOM::Element* ele = static_cast<DOM::Element*>(open_elements.back());
  ToyScopyUtil::logUtil("*** popped: %s", ele->tagName.c_str());
  open_elements.pop_back();
}

void HTMLDocumentParser::popOpenElementIf(std::string tagName) {
  DOM::Element* ele = static_cast<DOM::Element*>(open_elements.back());
  if (open_elements.size() == 0) return;
  if (ele->getTagName() != tagName) return;
  ToyScopyUtil::logUtil("*** popped: %s", ele->tagName.c_str());
  open_elements.pop_back();
}

void HTMLDocumentParser::popOpenElementIf(DOM::NodeType type) {
  if (open_elements.size() == 0) return;
  if (open_elements.back()->nodeType != type) return;
  DOM::Text* text = static_cast<DOM::Text*>(open_elements.back());
  ToyScopyUtil::logUtil("*** popped: %s", text->data.c_str());
  open_elements.pop_back();
}

void HTMLDocumentParser::setFramesetOkFlag(std::string str) {
  frameset_ok = str;
}

void HTMLDocumentParser::appendToCurrentNode(DOM::Node* n) {
  DOM::Element* ele = static_cast<DOM::Element*>(open_elements.back());
  DOM::Element* child = static_cast<DOM::Element*>(n);
  ToyScopyUtil::logUtil("append to %s", ele->tagName.c_str());
  ToyScopyUtil::logUtil("appended: %s", child->tagName.c_str());
  if (open_elements.size() > 0 &&
      open_elements.back()->nodeType == DOM::NodeType::TEXT_NODE) {
    popOpenElement();
  }
  ToyScopyUtil::logUtil("open last: %d", open_elements.back()->nodeType);
  open_elements.back()->appendChild(n);
}

bool HTMLDocumentParser::isToken(Tag::Token::Type type,
                                 Tag::Token::ElementType eleType) {
  Tag::Token* token = tokenizer->nextToken();
  return token->getTagType() == type && token->getTagElementType() == eleType;
}

bool HTMLDocumentParser::isToken(Tag::Token::ElementType type) {
  return tokenizer->nextToken()->getTagElementType() == type;
}

bool HTMLDocumentParser::isToken(Tag::Token::Type type) {
  return tokenizer->nextToken()->getTagType() == type;
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
  ToyScopyUtil::logUtil("### append character token");
  DOM::Node* node = findTextNode();
  if (node == NULL) {
    ToyScopyUtil::logUtil("create text node");
    node = document->createText("");
    ToyScopyUtil::logUtil("**********");
    DOM::Element* ele = static_cast<DOM::Element*>(open_elements.back());
    ToyScopyUtil::logUtil("%d", ele->nodeType);
    ToyScopyUtil::logUtil("%s", ele->tagName.c_str());
    open_elements.back()->appendChild(node);
    pushOpenElement(node);
  }
  DOM::Text* textNode = (static_cast<DOM::Text*>(node));
  textNode->appendData(data);
  // ToyScopyUtil::logUtil("%s", textNode->data.c_str());
}

void HTMLDocumentParser::setOriginalInsertionMode(Mode mode) {
  original_insertion_mode = mode;
}

void HTMLDocumentParser::appendAttributesToCurrentNode(DOM::Node* n) {
  DOM::Element* ele = static_cast<DOM::Element*>(n);

  Tag::Token* token = tokenizer->nextToken();
  for (auto a : token->getAttributes()) {
    ToyScopyUtil::logUtil("attr: %s : %s", a->getName().c_str(),
                          a->getValue().c_str());
    ele->setAttribute(a->getName(), a->getValue());
  }
}

void HTMLDocumentParser::stopParsing() {}
