#include "html_document_parser.h"
#include "dom.h"

#include <iostream>

void HTMLDocumentParser::parse()
{
  std::cout << "start parse" << std::endl;
  {
    // Initial State
    {
      this->consumeIgnoreToken();
      // TODO: comment token
      // https://html.spec.whatwg.org/multipage/parsing.html#the-initial-insertion-mode
      // A Comment Token

      // TODO: A DOCTYPE token
      this->insertion_mode = Mode::before_html;
    }

    std::cout << "before_html" << std::endl;
    // before html
    {
      // TODO: A DOCTYPE token
      // TODO: A Comment Token

      // Ignore the token
      this->consumeIgnoreToken();

      // A start tag whose tag name is "html"
      DOM::Element *element = new DOM::Element();
      Tag::Type type = Tag::Type::StartTag;
      int it = 0;
      char tagName[MAX_NAME];
      // pickup tag name
      if (this->doc[this->itr] == '<') {
        if (this->doc[this->itr + 1] == '/') {
          type = Tag::Type::EndTag, this->itr++;
        }
        while (this->doc[++this->itr] != '>') {
          tagName[it++] = this->doc[this->itr];
        }
        tagName[it] = '\0';
        element->tagName = tagName;

        // html tag
        if (type == Tag::Type::StartTag) {
          this->open_elements.push_back(element);
          this->document->appendChild(element);
        }
        // An end tag whose tag name is one of:
        // "head", "body", "html", "br"
        else if (type == Tag::Type::EndTag) {
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

  std::cout << "before_head" << std::endl;
  // TODO: The before_head
  {
  }

  std::cout << "text" << std::endl;
  // 12.2.6.4.8 text
  {
    char txt[1024];
    while (this->doc[this->itr] != '\0' && this->doc[this->itr] != '<') {
      consumeIgnoreToken();
      this->itr++;
      int it = 0;
      while (('a' <= this->doc[this->itr] && this->doc[this->itr] <= 'z') ||
             ('A' <= this->doc[this->itr] && this->doc[this->itr] <= 'Z') ||
             this->doc[this->itr] == ' ') {
        txt[it++] = this->doc[this->itr++];
        consumeIgnoreToken();
      }
    }
    DOM::Text *text = new DOM::Text(txt);
    std::cout << txt << std::endl;
    if (!open_elements.empty()) {
      std::cout << open_elements[0]->nodeType << std::endl;
      open_elements[0]->appendChild(text);
    }
  }

  // The end
  std::cout << "end" << std::endl;
  {
    this->document->readyState = DOM::DocumentReadyState::interactive;
    while (!this->open_elements.empty()) {
      this->open_elements.pop_back();
    }
  }
  std::cout << "____" << std::endl;
}

void HTMLDocumentParser::consumeIgnoreToken()
{
  // U+0009 CHARACTER TABULATION, U+000A LINE FEED (LF), U+000C FORM FEED
  // (FF), U+000D CARRIAGE RETURN (CR), or U+0020 SPACE
  while (doc[this->itr] == '\t' || doc[this->itr] == '\n' ||
         doc[this->itr] == '\f' || doc[this->itr] == '\r' ||
         // doc[this->itr] == ' ') {
         doc[this->itr] == '\f') {
    if (doc[this->itr] == '\0') break;
    this->itr++;
  }
}
