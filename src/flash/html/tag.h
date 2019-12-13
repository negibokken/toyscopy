#ifndef Tag_h
#define Tag_h

#include <iostream>
#include <string>
#define MAX_NAME 256

class Tag {
 public:
  // clang-format off
  enum Type {DOCTYPE, StartTag, EndTag, Comment, Character, };
  enum ElementType { html, head, body, div, h1, p, none };
  // clang-format on

  std::string tagName;
  Type type;
  ElementType elementType;
  std::string value;

  Tag(Type t = Type::StartTag) { type = t; }

  void print() { std::cout << type << ":" << tagName << std::endl; }

  static ElementType str2ElementType(std::string str) {
    if (str == "html") {
      return ElementType::html;
    }
    return ElementType::none;
  }

  void setTagType() {
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
    } else {
      this->elementType = ElementType::none;
    }
  }
  std::string getTagName() { return tagName; }
  ElementType getTagType() { return elementType; }

  void appendTagName(char c) { tagName += c; }
  void appendCharacter(char c) { value += c; }
};

#endif
