#ifndef TAG
#define TAG

#include <iostream>
#include <string>
#define MAX_NAME 256

class Tag {
  public:
  enum Type { StartTag, EndTag };

  std::string tagName;
  Type type;

  Tag() { type = Type::StartTag; }

  void print() { std::cout << type << ":" << tagName << std::endl; }
};

#endif
