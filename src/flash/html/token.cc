#include "token.h"

namespace Tag {

void Token::appendTagName(char c) {
  tagName += c;
  setTagType();
}

bool Token::hasAttribute(std::string attr) {
  for (auto a : attributes) {
    if (ToyScopyUtil::toASCIIlower(a->getName()) ==
        ToyScopyUtil::toASCIIlower(attr)) {
      return true;
    }
  }
  return false;
}

std::string Token::getAttributeValue(std::string attr) {
  std::string value;
  for (auto a : attributes) {
    if (ToyScopyUtil::toASCIIlower(a->getName()) ==
        ToyScopyUtil::toASCIIlower(attr)) {
      return a->getValue();
    }
  }
  return "";
}

Attribute* Token::currentAttribute() {
  if (attributes.empty()) attributes.push_back(new Attribute());
  return attributes.back();
}

void Token::setTagType() {
  if (tagName == "html") {
    elementType = ElementType::html;
  } else if (tagName == "head") {
    elementType = ElementType::head;
  } else if (tagName == "body") {
    elementType = ElementType::body;
  } else if (tagName == "p") {
    elementType = ElementType::p;
  } else if (tagName == "div") {
    elementType = ElementType::div;
  } else if (tagName == "h1") {
    elementType = ElementType::h1;
  } else if (tagName == "title") {
    elementType = ElementType::title;
  } else if (tagName == "a") {
    elementType = ElementType::a;
  } else if (tagName == "meta") {
    elementType = ElementType::meta;
  } else if (tagName == "style") {
    elementType = ElementType::style;
  } else {
    this->elementType = ElementType::none;
  }
}
}  // namespace Tag