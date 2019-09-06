#include "./html_document_parser.h"
#include <iostream>

void HTMLDocumentParser::parse(std::string str)
{
  std::cout << "hello html document parser" << std::endl;
  int i = 0;
  while (str[i] != '\0') {
    Tag tag;
    char c = str[i];
    int it = 0;
    char tagName[MAX_NAME];
    // pickup tag name
    if (c == '<') {
      if (str[i + 1] == '/') tag.type = Tag::Type::EndTag, i++;
      while (str[++i] != '>') {
        tagName[it++] = str[i];
      }
      tagName[it] = '\0';
      tag.tagName = tagName;
      tag.print();
    }
    i++;
  }
}

void HTMLDocumentParser::nextToken()
{
  std::cout << "hello next Token is here" << std::endl;
}
