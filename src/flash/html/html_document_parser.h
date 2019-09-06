#ifndef HTML_DOCUMENT_PARSER
#define HTML_DOCUMENT_PARSER

#include <stdio.h>
#include <string>

class HTMLDocumentParser {
  public:
  HTMLDocumentParser() {}
  void parse(std::string str);
  void nextToken();
};
#endif
