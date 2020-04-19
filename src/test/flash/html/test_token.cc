#include "../../../flash/html/token.h"
#include "gtest/gtest.h"

TEST(Token, TokenAttribute) {
  Tag::Attribute* attr = new Tag::Attribute();
  attr->setName("Content-Type");
  attr->setValue("text/html");
  EXPECT_STREQ("Content-Type", attr->getName().c_str());
  EXPECT_STREQ("text/html", attr->getValue().c_str());

  attr->appendName('2');
  attr->appendValue('2');
  EXPECT_STREQ("Content-Type2", attr->getName().c_str());
  EXPECT_STREQ("text/html2", attr->getValue().c_str());
}

TEST(Token, TokenTagType) {
  Tag::Token* t = new Tag::Token(Tag::Token::StartTag);

  EXPECT_STREQ("StartTag", Tag::Token::TagTypeName(t->getTagType()).c_str());

  std::string elementNames[] = {"html", "head",  "body",   "p",
                                "div",  "h1",    "title",  "a",
                                "meta", "style", "unknown"};

  for (auto elementName : elementNames) {
    Tag::Token* temp = new Tag::Token(Tag::Token::StartTag);
    for (auto c : elementName) {
      temp->appendTagName(c);
    }
  }
  t->appendTagName('b');
  t->appendTagName('o');
  t->appendTagName('d');
  t->appendTagName('y');
  EXPECT_STREQ("body", t->getTagName().c_str());
  EXPECT_EQ(Tag::Token::StartTag, t->getTagType());
  EXPECT_EQ(Tag::Token::body, t->getTagElementType());
}

TEST(Token, TokenAttributeOperation) {
  Tag::Token* t = new Tag::Token(Tag::Token::StartTag);
  t->createAttribute();
  std::string name = "Content-Type";
  std::string value = "text/html";
  for (auto c : name)
    t->appendAttributeName(c);
  for (auto c : value)
    t->appendAttributeValue(c);

  std::vector<Tag::Attribute*> attrs = t->getAttributes();

  EXPECT_EQ(true, t->hasAttribute("Content-Type"));
  EXPECT_EQ(false, t->hasAttribute("Content-Type2"));
  EXPECT_STREQ("Content-Type", attrs[0]->getName().c_str());
  EXPECT_STREQ("text/html", attrs[0]->getValue().c_str());
  EXPECT_STREQ("text/html", t->getAttributeValue("Content-Type").c_str());
  EXPECT_STREQ("text/html", t->getAttributeValue("content-type").c_str());
  EXPECT_STREQ("", t->getAttributeValue("no-exist-attr-name").c_str());

  EXPECT_EQ(1, attrs.size());
}
