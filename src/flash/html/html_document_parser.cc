#include "html_document_parser.h"

namespace Flash {

void HTMLDocumentParser::parse() {
  while (htmlTokenizer->pumpToken()) {
    if (!htmlTokenizer->canTakeNextToken())
      continue;

    Tag::Token* token = htmlTokenizer->nextToken();

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
          htmlTokenizer->consumeToken();
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
          htmlTokenizer->consumeToken();
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
          htmlTokenizer->consumeToken();
        } else {
          DOM::Node* n = this->document->createElement("head");
          head_pointer = n;
          setInsertionMode(Mode::in_head);
        }
        break;
      }
      case Mode::in_head: {
        ToyScopyUtil::logUtil("3");
        ToyScopyUtil::logUtil("=============");
        if (isToken(Tag::Token::Character)) {
          // FIXME:
          appendCharacterToken(token->getValue());
          htmlTokenizer->consumeToken();
        } else if (isToken(Tag::Token::Type::StartTag,
                           // if (isToken(Tag::Token::Type::StartTag,
                           Tag::Token::ElementType::title)) {
          DOM::Node* n = this->document->createElement("title");
          std::cout << "push" << std::endl;
          pushOpenElement(n);
          std::cout << "after" << std::endl;
          head_pointer->appendChild(n);
          std::cout << "after append" << std::endl;
          setOriginalInsertionMode(Mode::in_head);
          setInsertionMode(Mode::text);
          htmlTokenizer->consumeToken();
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
          htmlTokenizer->consumeToken();
        } else if (isToken(Tag::Token::Type::StartTag,
                           Tag::Token::ElementType::style)) {
          ToyScopyUtil::logUtil("style");
          // generic raw text element parsing algorihtm
          DOM::Node* n = this->document->createElement("style");
          head_pointer->appendChild(n);
          pushOpenElement(n);
          appendAttributesToCurrentNode(n);
          htmlTokenizer->setState(Flash::HTMLTokenizer::RAWTEXTState);
          setOriginalInsertionMode(Mode::in_head);
          setInsertionMode(Mode::text);
          htmlTokenizer->consumeToken();
        } else if (isToken(Tag::Token::Type::EndTag,
                           Tag::Token::ElementType::title)) {
          ToyScopyUtil::logUtil("pop title->");
          popOpenElementIf("title");
          htmlTokenizer->consumeToken();
        } else if (isToken(Tag::Token::Type::EndTag,
                           Tag::Token::ElementType::head)) {
          ToyScopyUtil::logUtil("go to after head");
          setInsertionMode(Mode::after_head);
          htmlTokenizer->consumeToken();
        } else {
          popOpenElementIf("head");
          setInsertionMode(Mode::after_head);
        }
        break;
      }
      case Mode::text: {
        if (isToken(Tag::Token::Type::Character)) {
          std::cout << "text: >" << token->getValue() << "<" << std::endl;
          appendCharacterToken(token->getValue());
          htmlTokenizer->consumeToken();
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
          htmlTokenizer->consumeToken();
        } else if (isToken(Tag::Token::Type::EndTag)) {
          // Text Node
          popOpenElementIf(DOM::NodeType::TEXT_NODE);
          // Some Element
          popOpenElement();
          setInsertionMode(original_insertion_mode);
          htmlTokenizer->consumeToken();
        }
        break;
      }
      case Mode::after_head: {
        ToyScopyUtil::logUtil("4");
        if (isToken(Tag::Token::Type::StartTag,
                    Tag::Token::ElementType::body)) {
          ToyScopyUtil::logUtil("body tag exists");
          DOM::Node* n = document->createElement("body");
          appendToCurrentNode(n);
          pushOpenElement(n);

          setFramesetOkFlag("not ok");

          setInsertionMode(Mode::in_body);
          htmlTokenizer->consumeToken();
        } else {
          ToyScopyUtil::logUtil("body tag not exists");
          DOM::Node* n = document->createElement("body");
          appendToCurrentNode(n);
          pushOpenElement(n);
          setInsertionMode(Mode::in_body);
        }
        break;
      }
      case Mode::in_body: {
        ToyScopyUtil::logUtil("5");
        if (isToken(Tag::Token::Type::Character)) {
          ToyScopyUtil::logUtil("character");
          appendCharacterToken(token->getValue());
          htmlTokenizer->consumeToken();
        } else if (isToken(Tag::Token::Type::EndTag,
                           Tag::Token::ElementType::body)) {
          popOpenElementIf("title");
          setInsertionMode(Mode::after_body);
          htmlTokenizer->consumeToken();
        } else if (isToken(Tag::Token::Type::EndTag)) {
          ToyScopyUtil::logUtil("end tag");
          popOpenElementIf(DOM::NodeType::TEXT_NODE);
          popOpenElement();
          htmlTokenizer->consumeToken();
        } else if (isToken(Tag::Token::Type::StartTag) &&
                   (isToken(Tag::Token::ElementType::h1) ||
                    isToken(Tag::Token::ElementType::h2) ||
                    isToken(Tag::Token::ElementType::h3) ||
                    isToken(Tag::Token::ElementType::h4) ||
                    isToken(Tag::Token::ElementType::h5) ||
                    isToken(Tag::Token::ElementType::h6))) {
          // TODO:  If the stack of open elements has a p element in button
          // scope, then close a p element.
          ToyScopyUtil::logUtil("start tag heading");
          DOM::Node* n = document->createElement(token->getTagName());
          appendAttributesToCurrentNode(n);
          appendToCurrentNode(n);
          pushOpenElement(n);
          htmlTokenizer->consumeToken();
        } else if (isToken(Tag::Token::Type::StartTag)) {
          ToyScopyUtil::logUtil("start tag");
          ToyScopyUtil::logUtil("tagname: %s", token->getTagName().c_str());
          DOM::Node* n = document->createElement(token->getTagName());
          appendAttributesToCurrentNode(n);
          appendToCurrentNode(n);
          pushOpenElement(n);
          htmlTokenizer->consumeToken();
        }
        break;
      }
      case Mode::after_body: {
        ToyScopyUtil::logUtil("6");
        if (isToken(Tag::Token::Type::EndTag, Tag::Token::ElementType::html)) {
          setInsertionMode(Mode::after_after_body);
          htmlTokenizer->consumeToken();
        }
        break;
      }
      case Mode::after_after_body: {
        ToyScopyUtil::logUtil("7");
        this->stopParsing();
        return;
      }
      default: {
        htmlTokenizer->consumeToken();
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
  ToyScopyUtil::logUtil("pushOpenElement");
  if (n == NULL || n == nullptr)
    return;
  ToyScopyUtil::logUtil("node-name: %s", n->nodeName.c_str());
  ToyScopyUtil::logUtil("node-type: %d", n->nodeType);
  if (n->nodeType == DOM::ELEMENT_NODE) {
    DOM::Element* ele = static_cast<DOM::Element*>(n);
    ToyScopyUtil::logUtil("*** pushed %s", ele->getTagName().c_str());
  } else if (n->nodeType == DOM::TEXT_NODE) {
    DOM::Text* ele = static_cast<DOM::Text*>(n);
    ToyScopyUtil::logUtil("*** pushed %s", ele->wholeText().c_str());
  }
  open_elements.push_back(n);
}

void HTMLDocumentParser::popOpenElement() {
  DOM::Node* last = lastOpenElement();
  if (last == nullptr)
    return;
  if (last->nodeType == DOM::ELEMENT_NODE) {
    DOM::Element* ele = static_cast<DOM::Element*>(last);
    ToyScopyUtil::logUtil("*** popped: %s", ele->getTagName().c_str());
  } else if (last->nodeType == DOM::TEXT_NODE) {
    DOM::Text* ele = static_cast<DOM::Text*>(last);
    ToyScopyUtil::logUtil("*** popped %s", ele->wholeText().c_str());
  }
  open_elements.pop_back();
}

void HTMLDocumentParser::popOpenElementIf(std::string tagName) {
  DOM::Node* last = lastOpenElement();
  if (last == nullptr)
    return;

  if (last->nodeType == DOM::ELEMENT_NODE) {
    DOM::Element* ele = static_cast<DOM::Element*>(last);
    if (ele->getTagName() != tagName)
      return;
    ToyScopyUtil::logUtil("*** popped: %s", ele->getTagName().c_str());
  } else if (last->nodeType == DOM::TEXT_NODE) {
    DOM::Text* ele = static_cast<DOM::Text*>(last);
    ToyScopyUtil::logUtil("*** popped %s", ele->wholeText().c_str());
  }

  open_elements.pop_back();
}

void HTMLDocumentParser::popOpenElementIf(DOM::NodeType type) {
  DOM::Node* last = lastOpenElement();
  if (last == nullptr)
    return;
  DOM::Text* text = static_cast<DOM::Text*>(last);
  if (last->nodeType != type)
    return;
  if (last->nodeType == DOM::ELEMENT_NODE) {
    DOM::Element* ele = static_cast<DOM::Element*>(last);
    ToyScopyUtil::logUtil("*** popped: %s", ele->getTagName().c_str());
  } else if (last->nodeType == DOM::TEXT_NODE) {
    DOM::Text* ele = static_cast<DOM::Text*>(last);
    ToyScopyUtil::logUtil("*** popped %s", ele->wholeText().c_str());
  }
  open_elements.pop_back();
}

void HTMLDocumentParser::setFramesetOkFlag(std::string str) {
  frameset_ok = str;
}

void HTMLDocumentParser::appendToCurrentNode(DOM::Node* n) {
  ToyScopyUtil::logUtil("appendToCurrentNode");
  DOM::Node* last = lastOpenElement();
  while (last != nullptr && last->nodeType == DOM::NodeType::TEXT_NODE) {
    popOpenElement();
    last = lastOpenElement();
  }
  if (last == nullptr)
    return;

  ToyScopyUtil::logUtil("here");
  ToyScopyUtil::logUtil("%d %s", last->nodeType, last->nodeName.c_str());
  if (last->nodeType == DOM::NodeType::ELEMENT_NODE) {
    DOM::Element* ele = static_cast<DOM::Element*>(last);
    ToyScopyUtil::logUtil("current node %s", ele->getTagName().c_str());
    DOM::Element* child = static_cast<DOM::Element*>(n);
    ToyScopyUtil::logUtil("append to %s", ele->getTagName().c_str());
    ToyScopyUtil::logUtil("appended: %s", child->getTagName().c_str());
    ToyScopyUtil::logUtil("open last: %d", open_elements.back()->nodeType);
    last->appendChild(n);
  }
}

bool HTMLDocumentParser::isToken(Tag::Token::Type type,
                                 Tag::Token::ElementType eleType) {
  Tag::Token* token = htmlTokenizer->nextToken();
  return token->getTagType() == type && token->getTagElementType() == eleType;
}

bool HTMLDocumentParser::isToken(Tag::Token::ElementType type) {
  return htmlTokenizer->nextToken()->getTagElementType() == type;
}

bool HTMLDocumentParser::isToken(Tag::Token::Type type) {
  return htmlTokenizer->nextToken()->getTagType() == type;
}

DOM::Node* HTMLDocumentParser::findTextNode() {
  for (int i = open_elements.size() - 1; i >= 0; i--) {
    if (open_elements[i]->nodeType == DOM::NodeType::TEXT_NODE) {
      ToyScopyUtil::logUtil("found text node!!");
      ToyScopyUtil::logUtil("%s", open_elements[i]->nodeName.c_str());
      ToyScopyUtil::logUtil(
          "%s", static_cast<DOM::Text*>(open_elements[i])->nodeName.c_str());
      ToyScopyUtil::logUtil(
          "%s", static_cast<DOM::Text*>(open_elements[i])->wholeText().c_str());
      return open_elements[i];
    }
  }
  ToyScopyUtil::logUtil("text node is not found");
  return NULL;
}

DOM::Node* HTMLDocumentParser::lastOpenElement() {
  if (open_elements.empty())
    return nullptr;
  return open_elements.back();
}

void HTMLDocumentParser::appendCharacterToken(std::string data) {
  ToyScopyUtil::logUtil("### append character token");
  DOM::Node* node = findTextNode();
  if (node == NULL) {
    ToyScopyUtil::logUtil("text node is null create text node");
    node = document->createText("");
    ToyScopyUtil::logUtil("**********");
    DOM::Node* last = lastOpenElement();
    if (last == nullptr) {
      ToyScopyUtil::logUtil("last node is empty so skipped");
      return;
    }
    DOM::Element* ele = static_cast<DOM::Element*>(last);
    ToyScopyUtil::logUtil("type: %d", ele->nodeType);
    ToyScopyUtil::logUtil("tag: %s", ele->getTagName().c_str());
    last->appendChild(node);
    // FIX Here
    pushOpenElement(node);
  }
  DOM::Text* textNode = (static_cast<DOM::Text*>(node));
  ToyScopyUtil::logUtil("node: >>%s<<", textNode->wholeText().c_str());
  textNode->appendData(data);
  // ToyScopyUtil::logUtil("%s", textNode->data.c_str());
}

void HTMLDocumentParser::setOriginalInsertionMode(Mode mode) {
  original_insertion_mode = mode;
}

void HTMLDocumentParser::appendAttributesToCurrentNode(DOM::Node* n) {
  ToyScopyUtil::logUtil("appendAttributesToCurrentNode");
  DOM::Element* ele = static_cast<DOM::Element*>(n);

  Tag::Token* token = htmlTokenizer->nextToken();
  for (auto a : token->getAttributes()) {
    ToyScopyUtil::logUtil("attr: %s : %s", a->getName().c_str(),
                          a->getValue().c_str());
    ele->setAttribute(a->getName(), a->getValue());
  }
}

void HTMLDocumentParser::stopParsing() {}

}  // namespace Flash