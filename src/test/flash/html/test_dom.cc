#include "../../../flash/html/dom.h"
#include "gtest/gtest.h"

TEST(DOM, DOMNodeappendChildDOM) {
  Flash::DOM::Node* parent =
      new Flash::DOM::Node(Flash::DOM::NodeType::DOCUMENT_NODE);
  Flash::DOM::Node* node =
      new Flash::DOM::Node(Flash::DOM::NodeType::ELEMENT_NODE);
  node->nodeName = "test";

  Flash::DOM::Node ret = parent->appendChild(node);
  EXPECT_EQ(parent->childNodes.size(), 1);
  EXPECT_EQ(parent->firstChild()->nodeType, Flash::DOM::NodeType::ELEMENT_NODE);
  EXPECT_EQ(parent->firstChild()->nodeName, ret.nodeName);
  EXPECT_EQ(parent->firstChild(), parent->lastChild());
}

TEST(DOM, DOMappendChildText) {
  Flash::DOM::Node* parent =
      new Flash::DOM::Node(Flash::DOM::NodeType::DOCUMENT_NODE);
  Flash::DOM::Text* text = new Flash::DOM::Text("hello");

  Flash::DOM::Node ret = parent->appendChild(text);
  EXPECT_EQ(ret.nodeType, Flash::DOM::NodeType::TEXT_NODE);
  EXPECT_EQ(parent->childNodes.size(), 1);
}

TEST(DOM, DOMnodeChild) {
  Flash::DOM::Node* parent =
      new Flash::DOM::Node(Flash::DOM::NodeType::DOCUMENT_NODE);

  EXPECT_EQ(parent->firstChild(), nullptr);
  EXPECT_EQ(parent->lastChild(), nullptr);
}

TEST(DOMNode, DOMNode) {
  Flash::DOM::Node* parent =
      new Flash::DOM::Node(Flash::DOM::NodeType::DOCUMENT_NODE);
  Flash::DOM::Node* node =
      new Flash::DOM::Node(Flash::DOM::NodeType::ELEMENT_NODE);
  node->nodeName = "test";

  Flash::DOM::Node ret = parent->appendChild(node);
  EXPECT_EQ(node->nodeType, Flash::DOM::NodeType::ELEMENT_NODE);
  EXPECT_EQ(node->nodeName, ret.nodeName);
  EXPECT_EQ(parent->childNodes.size(), 1);
  Flash::DOM::Node r = parent->appendChild(node);
}

TEST(DOM, DocumentType) {
  Flash::DOM::DocumentType* d =
      new Flash::DOM::DocumentType("name", "publicId", "systemId");
  EXPECT_STREQ(d->name.c_str(), "name");
  EXPECT_STREQ(d->publicId.c_str(), "publicId");
  EXPECT_STREQ(d->systemId.c_str(), "systemId");
}

TEST(DOMElement, ElementTest) {
  const std::string elementName = "meta";
  Flash::DOM::Element* element = new Flash::DOM::Element(elementName);
  element->setAttribute("http-equiv", "Content-Type");
  element->setAttribute("content", "text/html");
  element->setAttribute("charset", "utf-8");

  EXPECT_STREQ(element->getTagName().c_str(), elementName.c_str());
  EXPECT_EQ(element->getAttributeNames().size(), 3);
}

TEST(DOMText, TextElement) {
  Flash::DOM::Text* text = new Flash::DOM::Text("hello");

  EXPECT_STREQ(text->wholeText().c_str(), "hello");

  text->appendData(" world");
  EXPECT_STREQ(text->wholeText().c_str(), "hello world");
}

TEST(DOMDocument, DocumentTest) {
  Flash::DOM::Document* doc = new Flash::DOM::Document();

  Flash::DOM::Text* text = doc->createText("hello");
  Flash::DOM::Element* element = doc->createElement("body");

  EXPECT_STREQ(text->wholeText().c_str(), "hello");
  EXPECT_STREQ(element->getTagName().c_str(), "body");
}

TEST(DOMDocument, DocumentImplementation) {
  Flash::DOM::Document* doc = new Flash::DOM::Document();

  Flash::DOM::DocumentType* dtype =
      doc->implementation->createDocumentType("name", "publicId", "systemId");

  EXPECT_STREQ(dtype->name.c_str(), "name");
  EXPECT_STREQ(dtype->publicId.c_str(), "publicId");
  EXPECT_STREQ(dtype->systemId.c_str(), "systemId");
}