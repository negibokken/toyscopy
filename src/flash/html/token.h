#ifndef Tag_h
#define Tag_h

#include <iostream>
#include <string>
#include <vector>

#include "util.h"

namespace Tag {

class Attribute {
 public:
  Attribute() : name(""), value(""){};
  inline std::string getName() { return name; };
  inline void setName(std::string _name) { name = _name; };
  inline void appendName(char c) { name += c; };
  inline std::string getValue() { return value; };
  inline void setValue(std::string _value) { value = _value; };
  inline void appendValue(char c) { value += c; };

 private:
  std::string name;
  std::string value;
};

class Token {
 private:
  std::vector<Attribute*> attributes;

 public:
  // clang-format off
  enum Type { DOCTYPE, StartTag, EndTag, Comment, Character, };
  enum ElementType { html, head, meta, style, script, body, title, div, h1, p, a, none };
  // clang-format on
  static std::string TagTypeName(Type type) {
    const std::string names[] = {"DOCTYPE", "StartTag", "EndTag", "Comment",
                                 "Character"};
    return names[type];
  }

  std::string tagName;
  Type type;
  ElementType elementType;
  std::string value;

  void createAttribute() { attributes.push_back(new Attribute()); };
  Attribute* currentAttribute() { return attributes.back(); };
  void appendAttributeName(char c) {
    Attribute* a = currentAttribute();
    a->appendName(c);
  };
  void appendAttributeValue(char c) {
    // std::cout << c << std::endl;
    Attribute* a = currentAttribute();
    a->appendValue(c);
  };
  bool hasAttribute(std::string attr) {
    for (auto a : attributes) {
      if (ToyScopyUtil::toASCIIlower(a->getName()) ==
          ToyScopyUtil::toASCIIlower(attr)) {
        return true;
      }
    }
    return false;
  }
  std::string getAttributeValue(std::string attr) {
    std::string value;
    for (auto a : attributes) {
      if (ToyScopyUtil::toASCIIlower(a->getName()) ==
          ToyScopyUtil::toASCIIlower(attr)) {
        return a->getValue();
      }
    }
    return "";
  }
  std::vector<Attribute*> getAttributes() { return attributes; }

  Token(Type t = Type::StartTag) { type = t; }

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
  std::string getTagName() { return tagName; }
  Type getTagType() { return type; }
  ElementType getTagElementType() { return elementType; }

  void appendTagName(char c) { tagName += c; }
  void appendCharacter(char c) { value += c; }
};
}  // namespace Tag
#endif
