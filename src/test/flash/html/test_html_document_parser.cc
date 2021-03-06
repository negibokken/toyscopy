#include <queue>
#include <string>

#include "../../../flash/html/html_document_parser.h"
#include "../../test_util/test_util.cc"
#include "gtest/gtest.h"

TEST(HTMLDocumentParser, ParseVerySimpleHtml) {
  std::string str = "<html></html>";
  HTMLDocumentParser h(str);
  h.parse();
  std::string actual = dom2string(h.document, h.head_pointer);
  EXPECT_STREQ(actual.c_str(), str.c_str());
}

TEST(HTMLDocumentParser, ParseSimpleHtml) {
  std::string str = "<html>helloworld</html>";
  HTMLDocumentParser h(str);
  h.parse();
  std::string actual = dom2string(h.document, h.head_pointer);
  EXPECT_STREQ(actual.c_str(), str.c_str());
}

TEST(HTMLDocumentParser, ParseSimpleHtmlPlusBody) {
  std::string str = "<html><body>helloworld</body></html>";
  HTMLDocumentParser h(str);
  h.parse();
  std::string actual = dom2string(h.document, h.head_pointer);
  EXPECT_STREQ(actual.c_str(), str.c_str());
}

// test
TEST(HTMLDocumentParser, ParseStandard) {
  std::string str =
      "<!doctype html><html>"
      "<head>"
      "<meta charset=\"utf-8\"/>"
      "<meta http-equiv=\"Content-type\" content=\"text/html;charset=utf-8\"/>"
      "<meta name=\"viewport\" content=\"width=device-width,"
      "initial-scale=1\"/>"
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
      "</head>"
      "<body><div>"
      "<h1>ToyScopy</h1>"
      "<p>A tiny browser written in C++.</p>"
      "<h2>heading2</h2>"
      "<h3>heading3</h3>"
      "<h4>heading4</h4>"
      "<h5>heading5</h5>"
      "<h6>heading6</h6>"
      "<p> For more information, see here: "
      "<a href=\"https://github.com/negibokken/toyscopy\">"
      "https://github.com/negibokken/toyscopy"
      "</a>"
      "</p></div></body></html>";

  HTMLDocumentParser h(str);
  h.parse();

  std::queue<DOM::Node*> q;
  q.push(h.head_pointer);
  while (!q.empty()) {
    DOM::Node* node = q.front();
    q.pop();
    std::cout << node->nodeName << " " << std::endl;
    if (node->nodeType == DOM::NodeType::TEXT_NODE) {
      DOM::Text* txt = static_cast<DOM::Text*>(node);
      std::cout << txt->wholeText() << std::endl;
    } else if (node->nodeType == DOM::NodeType::ELEMENT_NODE) {
      DOM::Element* ele = static_cast<DOM::Element*>(node);
      std::cout << ele->getTagName() << std::endl;
    }
    for (auto child : node->childNodes) {
      q.push(child);
    }
  }

  std::string actual = dom2string(h.document, h.head_pointer);
  EXPECT_STREQ(actual.c_str(), str.c_str());
}
