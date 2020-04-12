#include <queue>
#include <string>

#include "../../../flash/html/html_document_parser.h"
#include "gtest/gtest.h"

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
        default: {
          break;
        }
      }
    }
  }
}

// test
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
        default: {
          break;
        }
      }
    }
  }
}
