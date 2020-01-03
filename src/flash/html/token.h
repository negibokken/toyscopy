#ifndef Tag_h
#define Tag_h

#include <iostream>
#include <string>
#include <vector>

#include "../../utils/log_util.h"
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
 public:
  // clang-format off
  enum Type { DOCTYPE, StartTag, EndTag, Comment, Character, };
  enum ElementType { html, head, meta, style, script, body, title, div, h1, p, a, none };
  // clang-format on

  Token(Type t = Type::StartTag) { type = t; }

  // methods related to Tag
  static std::string TagTypeName(Type type) {
    const std::vector<std::string> names = {"DOCTYPE", "StartTag", "EndTag",
                                            "Comment", "Character"};
    if (type < 0 || type >= names.size()) return "";
    return names[type];
  }
  inline std::string getTagName() { return tagName; }
  void appendTagName(char c);
  inline Type getTagType() { return type; }
  inline ElementType getTagElementType() { return elementType; }

  // methods related to Attribute
  inline void createAttribute() { attributes.push_back(new Attribute()); };
  inline void appendAttributeName(char c) {
    Attribute* a = currentAttribute();
    a->appendName(c);
  };
  inline void appendAttributeValue(char c) {
    Attribute* a = currentAttribute();
    a->appendValue(c);
  };
  bool hasAttribute(std::string attr);
  std::string getAttributeValue(std::string attr);
  inline std::vector<Attribute*> getAttributes() { return attributes; }

  // methods related to value
  inline std::string getValue() { return value; }
  void appendCharacter(char c) { value += c; }

 private:
  std::string tagName;
  Type type;
  ElementType elementType;
  std::string value;
  std::vector<Attribute*> attributes;

  Attribute* currentAttribute();
  void setTagType();
};
}  // namespace Tag
#endif
