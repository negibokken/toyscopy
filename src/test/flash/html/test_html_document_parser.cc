#include <string>
#include "../../../flash/html/html_document_parser.h"
#include "../../../flash/html/tag.h"
#include "gtest/gtest.h"

TEST(HTMLDocumentParser, ParseSimpleHtml)
{
  std::string str = "<html>hello</html>";
  HTMLDocumentParser h(str);
  h.parse();

  // Parse Token
  EXPECT_EQ(h.elements.size(), 1);
  // Element
  for (Tag::ElementType t : h.elements) {
    EXPECT_EQ(t, Tag::ElementType::html);
  }
}

TEST(example_test, html_document_parser)
{
  std::string str = "<html>hello</html>";
  HTMLDocumentParser h(str);
}
