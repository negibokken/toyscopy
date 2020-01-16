#include "../../../flash/html/html_document_parser.h"
#include "gtest/gtest.h"

#include <queue>
#include <string>

TEST(HTMLDocumentParser, ParseSimpleHtml) {
  std::string str = "<html>helloworld</html>";
  HTMLDocumentParser h(str);
  h.parse();

  std::queue<DOM::Node *> q;
  q.push(h.document);
  while (!q.empty()) {
    DOM::Node *node = q.front();
    q.pop();
    for (auto n : node->childNodes) {
      q.push(n);
      switch (n->nodeType) {
        case DOM::NodeType::ELEMENT_NODE: {
          DOM::Element *ele = reinterpret_cast<DOM::Element *>(n);
          std::cout << ele->nodeType << std::endl;
          std::cout << ele->getTagName() << std::endl;
          break;
        }
        case DOM::NodeType::TEXT_NODE: {
          std::cout << n->nodeType << std::endl;
          std::cout << (reinterpret_cast<DOM::Text *>(n)->wholeText())
                    << std::endl;
          break;
        }
        default: { break; }
      }
    }
  }
}

TEST(HTMLDocumentParser, ParseSimpleHtmlPlusBody) {
  std::string str = "<html><body>helloworld</body></html>";
  HTMLDocumentParser h(str);
  h.parse();

  std::queue<DOM::Node *> q;
  q.push(h.document);
  while (!q.empty()) {
    DOM::Node *node = q.front();
    q.pop();
    for (auto n : node->childNodes) {
      q.push(n);
      switch (n->nodeType) {
        case DOM::NodeType::ELEMENT_NODE: {
          DOM::Element *ele = reinterpret_cast<DOM::Element *>(n);
          std::cout << ele->nodeType << std::endl;
          std::cout << ele->getTagName() << std::endl;
          break;
        }
        case DOM::NodeType::TEXT_NODE: {
          std::cout << n->nodeType << std::endl;
          std::cout << (reinterpret_cast<DOM::Text *>(n)->wholeText())
                    << std::endl;
          break;
        }
        default: { break; }
      }
    }
  }
}

TEST(HTMLDocumentParser, ParseStandard) {
  std::string str =
      "<!doctype html><html><head>"
      "<meta charset=\"utf-8\" />"
      "<meta http-equiv=\"Content-type\" content=\"text/html; charset=utf-8\""
      "/>"
      "<meta name=\"viewport\" content=\"width=device-width,"
      "initial-scale=1\" "
      "/>"
      "<title>About ToyScopy</title>"
      "<style type=\"text/css\">"
      "    body {"
      "        background-color: #f0f0f2;"
      "        margin: 0;"
      "        padding: 0;"
      "    }"
      "    div {"
      "        width: 600px;"
      "        margin: 5em auto;"
      "        padding: 2em;"
      "        background-color: #fdfdff;"
      "        border-radius: 0.5em;"
      "        box-shadow: 2px 3px 7px 2px rgba(0,0,0,0.02);"
      "    }"
      "</style>"
      "</head><body><div>"
      "<h1>ToyScopy</h1>"
      "<p>A tiny browser written in C++.</p>"
      "<p><a href=\"https://github.com/negibokken/toyscopy\">"
      "For more information, see here: https://github.com/negibokken/toyscopy"
      "</a></p></div></body></html>";

  HTMLDocumentParser h(str);
  h.parse();

  std::queue<DOM::Node *> q;
  q.push(h.document);
  while (!q.empty()) {
    DOM::Node *node = q.front();
    q.pop();
    for (auto n : node->childNodes) {
      q.push(n);
      switch (n->nodeType) {
        case DOM::NodeType::ELEMENT_NODE: {
          DOM::Element *ele = reinterpret_cast<DOM::Element *>(n);
          std::cout << ele->nodeType << std::endl;
          std::cout << ele->getTagName() << std::endl;
          break;
        }
        case DOM::NodeType::TEXT_NODE: {
          std::cout << n->nodeType << std::endl;
          std::cout << (reinterpret_cast<DOM::Text *>(n)->wholeText())
                    << std::endl;
          break;
        }
        default: { break; }
      }
    }
  }
}
