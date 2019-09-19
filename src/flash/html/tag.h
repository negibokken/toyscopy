#ifndef TAG
#define TAG

#include <iostream>
#include <string>
#define MAX_NAME 256

class Tag {
  public:
  // clang-format off
  enum Type { StartTag, EndTag };
  enum ElementType { html, none };
  // clang-format on

  std::string tagName;
  Type type;
  ElementType elementType;

  Tag() { type = Type::StartTag; }

  void print() { std::cout << type << ":" << tagName << std::endl; }

  static ElementType str2ElementType(std::string str)
  {
    if (str == "html") {
      return ElementType::html;
    }
    return ElementType::none;
  }

  void setTagName(std::string str)
  {
    this->tagName = str;
    if (str == "html") {
      this->elementType = ElementType::html;
    }
    else {
      this->elementType = ElementType::none;
    }
  }
};

#endif
