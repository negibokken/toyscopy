#ifndef Tag_h
#define Tag_h

#include <iostream>
#include <string>
#define MAX_NAME 256

class Tag {
 public:
  // clang-format off
  enum Type { StartTag, Character, EndTag };
  enum ElementType { html, head, body, none };
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
    } else {
      this->elementType = ElementType::none;
    }
  }
  ElementType getTagType() { return elementType; }

  void appendTagName(char c) {
    // std::cout << c << std::endl;
    tagName += c;
  }
  void appendCharacter(char c) { value += c; }
};

#endif
