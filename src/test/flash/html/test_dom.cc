#include "../../../flash/html/dom.h"
#include "gtest/gtest.h"

TEST(DOM, DOMappendChildDOM)
{
  DOM::Node *parent = new DOM::Node(DOM::NodeType::DOCUMENT_NODE);
  DOM::Node *node = new DOM::Node(DOM::NodeType::ELEMENT_NODE);
  node->nodeName = "test";

  DOM::Node ret = parent->appendChild(node);
  EXPECT_EQ(node->nodeType, DOM::NodeType::ELEMENT_NODE);
  EXPECT_EQ(node->nodeName, ret.nodeName);
  EXPECT_EQ(parent->childNodes.size(), 1);
}

TEST(DOM, DOMappendChildText)
{
  DOM::Node *parent = new DOM::Node(DOM::NodeType::DOCUMENT_NODE);
  DOM::Text *text = new DOM::Text("hello");

  DOM::Node ret = parent->appendChild(text);
  EXPECT_EQ(ret.nodeType, DOM::NodeType::TEXT_NODE);
  EXPECT_EQ(parent->childNodes.size(), 1);
}
