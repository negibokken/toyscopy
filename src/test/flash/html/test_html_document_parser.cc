#include <string>
#include "../../../flash/html/html_document_parser.h"
#include "gtest/gtest.h"

TEST(example_test, html_document_parser)
{
  std::string str = "<html>hello</html>";
  HTMLDocumentParser h;
  h.parse(str);

  h.nextToken();
}
