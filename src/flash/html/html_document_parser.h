#ifndef HTML_DOCUMENT_PARSER
#define HTML_DOCUMENT_PARSER

#include <stdio.h>
#include <string>
#include "tag.h"

class HTMLDocumentParser {
  public:
  enum State {

  };
  HTMLDocumentParser() {}
  void parse(std::string str);
  void nextToken();
};
#endif
