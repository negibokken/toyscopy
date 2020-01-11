#include "../../../flash/html/dom.h"
#include "gtest/gtest.h"

TEST(DOM, DOMNodeappendChildDOM) {
  DOM::Node *parent = new DOM::Node(DOM::NodeType::DOCUMENT_NODE);
  DOM::Node *node = new DOM::Node(DOM::NodeType::ELEMENT_NODE);
  node->nodeName = "test";

  DOM::Node ret = parent->appendChild(node);
  EXPECT_EQ(parent->childNodes.size(), 1);
  EXPECT_EQ(parent->firstChild()->nodeType, DOM::NodeType::ELEMENT_NODE);
  EXPECT_EQ(parent->firstChild()->nodeName, ret.nodeName);
  EXPECT_EQ(parent->firstChild(), parent->lastChild());
}

TEST(DOM, DOMappendChildText) {
  DOM::Node *parent = new DOM::Node(DOM::NodeType::DOCUMENT_NODE);
  DOM::Text *text = new DOM::Text("hello");

  DOM::Node ret = parent->appendChild(text);
  EXPECT_EQ(ret.nodeType, DOM::NodeType::TEXT_NODE);
  EXPECT_EQ(parent->childNodes.size(), 1);
}

TEST(DOM, DOMnodeChild) {
  DOM::Node *parent = new DOM::Node(DOM::NodeType::DOCUMENT_NODE);

  EXPECT_EQ(parent->firstChild(), nullptr);
  EXPECT_EQ(parent->lastChild(), nullptr);
}

TEST(DOMNode, DOMNode) {
  DOM::Node *parent = new DOM::Node(DOM::NodeType::DOCUMENT_NODE);
  DOM::Node *node = new DOM::Node(DOM::NodeType::ELEMENT_NODE);
  node->nodeName = "test";

  DOM::Node ret = parent->appendChild(node);
  EXPECT_EQ(node->nodeType, DOM::NodeType::ELEMENT_NODE);
  EXPECT_EQ(node->nodeName, ret.nodeName);
  EXPECT_EQ(parent->childNodes.size(), 1);
  DOM::Node r = parent->appendChild(node);
}

TEST(DOM, DocumentType) {
  DOM::DocumentType *d = new DOM::DocumentType("name", "publicId", "systemId");
  EXPECT_STREQ(d->name.c_str(), "name");
  EXPECT_STREQ(d->publicId.c_str(), "publicId");
  EXPECT_STREQ(d->systemId.c_str(), "systemId");
}

TEST(DOMElement, ElementTest) {
  const std::string elementName = "meta";
  DOM::Element *element = new DOM::Element(elementName);
  element->setAttribute("http-equiv", "Content-Type");
  element->setAttribute("content", "text/html");
  element->setAttribute("charset", "utf-8");

  EXPECT_STREQ(element->getTagName().c_str(), elementName.c_str());
  EXPECT_EQ(element->getAttributeNames().size(), 3);
}

TEST(DOMText, TextElement) {
  DOM::Text *text = new DOM::Text("hello");

  EXPECT_STREQ(text->wholeText().c_str(), "hello");

  text->appendData(" world");
  EXPECT_STREQ(text->wholeText().c_str(), "hello world");
}

TEST(DOMDocument, DocumentTest) {
  DOM::Document *doc = new DOM::Document();

  DOM::Text *text = doc->createText("hello");
  DOM::Element *element = doc->createElement("body");

  EXPECT_STREQ(text->wholeText().c_str(), "hello");
  EXPECT_STREQ(element->getTagName().c_str(), "body");
}

TEST(DOMDocument, DocumentImplementation) {
  DOM::Document *doc = new DOM::Document();

  DOM::DocumentType *dtype =
      doc->implementation->createDocumentType("name", "publicId", "systemId");

  EXPECT_STREQ(dtype->name.c_str(), "name");
  EXPECT_STREQ(dtype->publicId.c_str(), "publicId");
  EXPECT_STREQ(dtype->systemId.c_str(), "systemId");
}