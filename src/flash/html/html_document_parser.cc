#include "./html_document_parser.h"
#include <iostream>

void HTMLDocumentParser::parse()
{
  this->itr = 0;
  while (this->doc[this->itr] != '\0') {
    // Initial State
    {
      this->consumeIgnoreToken();
      // TODO: comment token
      // https://html.spec.whatwg.org/multipage/parsing.html#the-initial-insertion-mode
      // A Comment Token

      // TODO: A DOCTYPE token
      this->insertion_mode = Mode::before_html;
    }

    // before html
    {
      // TODO: A DOCTYPE token
      // TODO: A Comment Token

      // Ignore the token
      this->consumeIgnoreToken();

      // A start tag whose tag name is "html"
      Tag tag;
      int it = 0;
      char tagName[MAX_NAME];
      // pickup tag name
      if (this->doc[this->itr] == '<') {
        if (this->doc[this->itr + 1] == '/')
          tag.type = Tag::Type::EndTag, this->itr++;
        while (this->doc[++this->itr] != '>') {
          tagName[it++] = this->doc[this->itr];
        }
        tagName[it] = '\0';
        tag.tagName = tagName;

        // html tag
        if (tag.type == Tag::Type::StartTag) {
          this->elements.push_back(Tag::str2ElementType(tag.tagName));
        }
        // An end tag whose tag name is one of:
        // "head", "body", "html", "br"
        else if (tag.type == Tag::Type::EndTag) {
          // "head", "body", "html", "br"
          if (!strcmp(tagName, "head") || !strcmp(tagName, "body") ||
              !strcmp(tagName, "html") || !strcmp(tagName, "br")) {
            // TODO: An end tag whose tag name is one of: "head", "body",
            // "html", "br" act as anything else
          }
          else {
            // Parse Error Ignore token
          }
        }
        // TODO:: Anything else
        else {
        }
      }
    }
  }

  // The before_head
  {
  }
}

void HTMLDocumentParser::consumeIgnoreToken()
{
  // U+0009 CHARACTER TABULATION, U+000A LINE FEED (LF), U+000C FORM FEED
  // (FF), U+000D CARRIAGE RETURN (CR), or U+0020 SPACE
  while (doc[this->itr] == '\t' || doc[this->itr] == '\n' ||
         doc[this->itr] == '\f' || doc[this->itr] == '\r' ||
         doc[this->itr] == ' ') {
    this->itr++;
  }
}
