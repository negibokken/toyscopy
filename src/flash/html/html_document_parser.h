#ifndef HTML_DOCUMENT_PARSER
#define HTML_DOCUMENT_PARSER

#include <stdio.h>
#include <string>
#include <vector>
#include "dom.h"
#include "tag.h"

class HTMLDocumentParser
{
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
  HTMLDocumentParser(std::string _doc) { this->doc = _doc; };

  int *head = NULL;
  int itr = 0;
  std::string doc;
  Mode insertion_mode;
  std::vector<DOM::Node *> open_elements;

  DOM::Document document;

  void parse();
  void consumeIgnoreToken();
};
#endif
