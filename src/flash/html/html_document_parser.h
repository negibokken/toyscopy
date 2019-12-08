#ifndef HTML_Document_Parser_h
#define HTML_Document_Parser_h

#include <stdio.h>

#include <string>
#include <vector>

#include "dom.h"
#include "tokenizer.h"

class HTMLDocumentParser {
 public:
  // described here:
  // https://html.spec.whatwg.org/multipage/parsing.html#parse-state
  // clang-format off
  enum Mode {
    initial, before_html, before_head, in_head, in_head_noscript, after_head,
    in_body, text, in_table, in_table_text, in_caption, in_column_group,
    in_table_body, in_row, in_cell, in_select, in_template, after_body,
    in_frameset, after_frameset, after_after_body, after_after_frameset
  };
  // clang-format on
  HTMLDocumentParser(std::string _doc) {
    this->doc = _doc;
    this->document = new DOM::Document();
    this->frameset_ok = "ok";
    this->tokenizer = new Tokenizer::Tokenizer(this->doc);
    this->insertion_mode = Mode::initial;
  };

  int *head = NULL;
  int itr = 0;
  std::string doc;
  Mode insertion_mode;
  std::vector<DOM::Node *> open_elements;

  DOM::Document *document;
  std::string frameset_ok;
  Tokenizer::Tokenizer *tokenizer;
  DOM::Node *head_pointer;

  void parse();
  void consumeIgnoreToken();
  void consumeToken();
  void setInsertionMode(Mode mode);
  void pushOpenElement(DOM::Node *n);
  void popOpenElement();
  void setFramesetOkFlag(std::string str);
  void appendToCurrentNode(DOM::Node *n);
  bool isToken(Tag::Type type, Tag::ElementType eleType);
  bool isToken(Tag::Type type);
  void appendCharacterToken(std::string data);
  void stopParsing();
  DOM::Node *findTextNode();
};

#endif
