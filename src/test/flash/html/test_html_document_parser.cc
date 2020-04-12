#include <queue>
#include <string>

#include "../../../flash/html/html_document_parser.h"
#include "../../test_util/test_util.cc"
#include "gtest/gtest.h"

TEST(HTMLDocumentParser, ParseVerySimpleHtml) {
  std::string str = "<html></html>";
  HTMLDocumentParser h(str);
  h.parse();
  std::string actual = dom2string(h.document);
  EXPECT_STREQ(actual.c_str(), str.c_str());
}

TEST(HTMLDocumentParser, ParseSimpleHtml) {
  std::string str = "<html>helloworld</html>";
  HTMLDocumentParser h(str);
  h.parse();
  std::string actual = dom2string(h.document);
  EXPECT_STREQ(actual.c_str(), str.c_str());
}

TEST(HTMLDocumentParser, ParseSimpleHtmlPlusBody) {
  std::string str = "<html><body>helloworld</body></html>";
  HTMLDocumentParser h(str);
  h.parse();
  std::string actual = dom2string(h.document);
  EXPECT_STREQ(actual.c_str(), str.c_str());
}

// test
TEST(HTMLDocumentParser, ParseStandard) {
  std::string str =
      "<!doctype html><html>"
      "<head>"
      // "<meta charset=\"utf-8\" />"
      // "<meta http-equiv=\"Content-type\" content=\"text/html;
      // charset=utf-8\"/>"
      // "<meta name=\"viewport\" content=\"width=device-width,"
      // "initial-scale=1\" "
      // "/>"
      // "<title>About ToyScopy</title>"
      // "<style type=\"text/css\">"
      // "    body {"
      // "        background-color: #f0f0f2;"
      // "        margin: 0;"
      // "        padding: 0;"
      // "    }"
      // "    div {"
      // "        width: 600px;"
      // "        margin: 5em auto;"
      // "        padding: 2em;"
      // "        background-color: #fdfdff;"
      // "        border-radius: 0.5em;"
      // "        box-shadow: 2px 3px 7px 2px rgba(0,0,0,0.02);"
      // "    }"
      // "</style>"
      "</head>"
      "<body><div>"
      "<h1>ToyScopy</h1>"
      "<p>A tiny browser written in C++.</p>"
      "<p><a href=\"https://github.com/negibokken/toyscopy\">"
      "For more information, see here: https://github.com/negibokken/toyscopy"
      "</a></p></div></body></html>";

  HTMLDocumentParser h(str);
  h.parse();

  std::string actual = dom2string(h.document);
  std::cout << actual << std::endl;
}
