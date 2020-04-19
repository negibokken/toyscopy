#ifndef HTML_Document_Parser_h
#define HTML_Document_Parser_h

#include <stdio.h>

#include <string>
#include <vector>

#include "../../utils/log_util.h"
#include "dom.h"
#include "token.h"
#include "tokenizer.h"
#include "util.h"

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

  DOM::Document* document;
  DOM::Node* head_pointer;
  HTMLDocumentParser(std::string _doc) {
    ToyScopyUtil::logUtil("loaded document");
    ToyScopyUtil::logUtil("%s", _doc.c_str());
    this->doc = _doc;
    this->document = new DOM::Document();
    this->frameset_ok = "ok";
    this->tokenizer = new Tokenizer::Tokenizer(this->doc);
    this->insertion_mode = Mode::initial;
  };
  void parse();

  inline void setDocumentTitle(const std::string title) {
    document_title = title;
  };
  inline std::string getDocumentTitle() const { return document_title; };

 private:
  std::string doc = "";
  std::string charset = "";
  std::string* characterSet = &charset;
  std::string* inputEncoding = &charset;
  std::string contentType = "";

  Mode insertion_mode;
  Mode original_insertion_mode;
  std::vector<DOM::Node*> open_elements = std::vector<DOM::Node*>(0);

  std::string frameset_ok = "";
  Tokenizer::Tokenizer* tokenizer;

  std::string document_title = "";

  void consumeIgnoreToken();
  void consumeToken();
  void setInsertionMode(Mode mode);
  void setOriginalInsertionMode(Mode mode);
  void pushOpenElement(DOM::Node* n);
  void popOpenElement();
  void popOpenElementIf(std::string eleType);
  void popOpenElementIf(DOM::NodeType type);
  void setFramesetOkFlag(std::string str);
  void appendToCurrentNode(DOM::Node* n);
  void appendAttributesToCurrentNode(DOM::Node* n);
  bool isToken(Tag::Token::Type type, Tag::Token::ElementType eleType);
  bool isToken(Tag::Token::Type type);
  bool isToken(Tag::Token::ElementType type);
  void appendCharacterToken(std::string data);
  void stopParsing();
  DOM::Node* lastOpenElement();
  DOM::Node* findTextNode();
};

#endif
