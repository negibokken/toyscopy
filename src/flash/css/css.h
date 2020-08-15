#ifndef CCS_h
#define CCS_h

#include <string>
#include <vector>
#include "../html/dom.h"

namespace Flash {
namespace CSS {

enum Style { INLINE, NONE, BLOCK, INLINE_BLOCK, LIST_ITEM };

class CSSRule;
class CSSStyleSheet;
class CSSRuleList;
class MediaList;

class CSSOM {};

class CSSRule {
 public:
  const unsigned short STYLE_RULE = 1;
  const unsigned short CHARSET_RULE = 2;
  const unsigned short IMPORT_RULE = 3;
  const unsigned short MEDIA_RULE = 4;
  const unsigned short FONT_FACE_RULE = 5;
  const unsigned short PAGE_RULE = 6;
  const unsigned short MARGIN_RULE = 9;
  const unsigned short NAMESPACE_RULE = 10;

 private:
  unsigned short type;
  std::string cssText;
  CSSRule* parentRule;
  CSSStyleSheet* parentStyleSheet;
  CSSRuleList* children;

 public:
  CSSRule();
  ~CSSRule();
  inline unsigned short getType() const { return type; }
  inline std::string getCSSText() const { return cssText; }
  inline void setCSSText(std::string cssText) { cssText = cssText; }
  inline const CSSRule* getParentRule() const { return parentRule; }
  inline const CSSStyleSheet* getStyleSheet() const { return parentStyleSheet; }
};

class CSSStyleDeclaration {
 private:
  std::string cssText;
  unsigned long length;
  CSSRule* parentRule;
  std::string cssFloat;

 public:
  CSSStyleDeclaration(/* args */);
  ~CSSStyleDeclaration();
  inline unsigned long getLength() { return length; }
  inline std::string item(unsigned long index) {}
  std::string setProperty(std::string property, std::string value);
  std::string setProperty(std::string property,
                          std::string value,
                          std::string priority);
  std::string getPropertyValue(std::string property);
  std::string getPropertyPriority(std::string property);
  void setPropertyValue(std::string property, std::string value);
  void setPropertyPriority(std::string property, std::string priority);
  std::string removeProperty(std::string property);
  inline CSSRule* getParentRule() const { return parentRule; };
  inline std::string getCSSFloat() { return cssFloat; }
  inline void setCSSFloat(std::string cssFloat) { cssFloat = cssFloat; }
};

CSSStyleDeclaration::CSSStyleDeclaration(/* args */) {}

CSSStyleDeclaration::~CSSStyleDeclaration() {}

class CSSStyleRule : public CSSRule {
 private:
  std::string selectorText;

 public:
  CSSStyleRule();
  ~CSSStyleRule();
  inline const std::string getSelectorText() const { return selectorText; }

}

class CSSImportRule : public CSSRule {
 private:
  std::string href;
  MediaList* media;
  CSSStyleSheet* styleSheet;

 public:
  CSSImportRule();
  ~CSSImportRule();
  inline const std::string getHref() const { return href; }
  inline const MediaList* getMediaList() const { return media; }
  inline const CSSStyleSheet* getStyleSheet() const { return styleSheet; }
};

class CSSRuleList {
 private:
  std::vector<StyleSheet*> items;
  unsigned long length;

 public:
  CSSRuleList() : length(0){};
  ~CSSRuleList();
  inline CSSRule* item(unsigned long index) {
    if (index >= items.size())
      return nullptr;
    return items[index];
  }
  void append(CSSRule* cssRule) {
    length++;
    items.push_back(cssRule);
  }
};

class MediaList {
 private:
  std::string mediaText;
  unsigned long length;
  std::vector<std::string> items;

 public:
  MediaList();
  ~MediaList();
  std::string item(unsigned long index);
  void appendMedium(std::string medium);
  void deleteMedium(std::string medium);
};

// Ref: https://www.w3.org/TR/cssom-1/#the-stylesheet-interface
class StyleSheet {
 private:
  std::string type;
  std::string href;
  DOM::Node* ownerNode;
  StyleSheet* parentStyleSheet;
  std::string title;
  MediaList* media;
  bool disabled;

 public:
  StyleSheet();
  ~StyleSheet();
  inline std::string getType() { return type; }
  inline std::string getHref() { return href; }
  inline DOM::Node* getOwnerNode() { return ownerNode; }
  inline StyleSheet* getParentStyleSheet() { return parentStyleSheet; }
  inline std::string getTitle() { return title; }
  inline MediaList* getMedia() { return media; }
  inline bool getDisabled() { return disabled; }
  inline void setDisabled(bool diabled) { diabled = diabled; }
};

class CSSStyleSheet : public StyleSheet {
 public:
  CSSStyleSheet();
  ~CSSStyleSheet();
};

class StyleSheetList {
 private:
  std::vector<StyleSheet*> items;
  unsigned long length;

 public:
  StyleSheetList() : length(0){};
  ~StyleSheetList();
  inline StyleSheet* item(unsigned long index) {
    if (index >= items.size())
      return nullptr;
    return items[index];
  }
  void append(StyleSheet* styleSheet) {
    length++;
    items.push_back(styleSheet);
  }
};

}  // namespace CSS

}  // namespace Flash

#endif
