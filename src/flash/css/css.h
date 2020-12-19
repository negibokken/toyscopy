#ifndef CCS_h
#define CCS_h

#include <string>
#include <unordered_map>
#include <vector>

#include "../html/dom.h"

namespace Flash {
namespace CSS {

enum Style { INLINE, NONE, BLOCK, INLINE_BLOCK, LIST_ITEM };

class CSSRule;
class CSSStyleRule;
class CSSStyleSheet;
class CSSRuleList;
class MediaList;
class CSSStyleDeclaration;

class CSSOM {};

class CSSRule {
 public:
  static const unsigned short STYLE_RULE = 1;
  static const unsigned short CHARSET_RULE = 2;
  static const unsigned short IMPORT_RULE = 3;
  static const unsigned short MEDIA_RULE = 4;
  static const unsigned short FONT_FACE_RULE = 5;
  static const unsigned short PAGE_RULE = 6;
  static const unsigned short MARGIN_RULE = 9;
  static const unsigned short NAMESPACE_RULE = 10;

 private:
  unsigned short type;
  std::string cssText;
  CSSRule* parentRule;
  CSSStyleSheet* parentStyleSheet;
  CSSRuleList* children;

 public:
  CSSRule(unsigned short type) : type(type){};
  ~CSSRule(){};
  inline unsigned short getType() const { return type; }
  inline std::string getCSSText() const { return cssText; }
  inline void setCSSText(std::string cssText) { cssText = cssText; }
  inline const CSSRule* getParentRule() const { return parentRule; }
  inline const CSSStyleSheet* getStyleSheet() const { return parentStyleSheet; }
};

class CSSStyleRule : public CSSRule {
 private:
  std::string selectorText;
  CSSStyleDeclaration* declaration;

 public:
  CSSStyleRule() : CSSRule(CSSRule::STYLE_RULE), selectorText("") {}
  CSSStyleRule(CSSRule* rule) : CSSRule(CSSRule::STYLE_RULE) {
    auto text = rule->getCSSText();
    this->selectorText = text;
  };
  ~CSSStyleRule();
  inline const std::string getSelectorText() const { return selectorText; }
  inline void setSelectorText(std::string selector) { selectorText = selector; }
  void appendDeclarations(CSSStyleDeclaration* dec);
};

class CSSRuleFactory {
 public:
  static CSSRule* createCSSRule(unsigned short type) {
    switch (type) {
      case CSSRule::STYLE_RULE: {
        return new CSSStyleRule();
      }
      default: {
        return new CSSStyleRule();
      }
    }
  }
};

class CSSStyleDeclaration {
 private:
  std::string cssText;
  unsigned long length;
  CSSRule* parentRule;
  std::string cssFloat;
  std::unordered_map<std::string, std::string> properties;
  std::unordered_map<std::string, std::string> propertyPriorities;

 public:
  CSSStyleDeclaration(/* args */) : parentRule(nullptr){};
  ~CSSStyleDeclaration() { delete parentRule; };
  inline unsigned long getLength() { return length; }
  inline std::string item(unsigned long index) {
    std::unordered_map<std::string, std::string>::iterator it =
        properties.begin();
    for (int i = 0; i < index; i++)
      it++;
    return it->first;
  };
  void setProperty(std::string property, std::string value) {
    properties[property] = value;
  };
  void setProperty(std::string property,
                   std::string value,
                   std::string priority) {
    properties[property] = value;
    propertyPriorities[property] = priority;
  };
  std::string getPropertyValue(std::string property) {
    if (properties.count(property) == 0) {
      return "";
    }
    return properties[property];
  };
  std::string getPropertyPriority(std::string property) {
    if (propertyPriorities.count(property) == 0) {
      return "";
    }
    return propertyPriorities[property];
  };
  void setPropertyValue(std::string property, std::string value) {
    properties[property] = value;
  };
  void setPropertyPriority(std::string property, std::string priority) {
    propertyPriorities[property] = priority;
  };
  std::string removeProperty(std::string property) {
    if (properties.count(property) == 0) {
      return "";
    }
    std::string s = properties[property];
    properties.erase(property);
    propertyPriorities.erase(property);
    return s;
  };
  std::vector<std::string> getPropertyKeys() {
    std::vector<std::string> keys(properties.size());
    int i = 0;
    for (auto const& a : properties) {
      keys[i++] = a.first;
    }
    return keys;
  }
  inline CSSRule* getParentRule() const { return parentRule; };
  inline std::string getCSSFloat() { return cssFloat; }
  inline void setCSSFloat(std::string cssFloat) { cssFloat = cssFloat; }
};

class CSSStyleDeclarationBlock {
 private:
  bool readOnlyFlag;
  std::vector<CSSStyleDeclaration*> declarations;
  CSSStyleRule* parentRule;
  Flash::DOM::Element* ownerNode;

 public:
  CSSStyleDeclarationBlock()
      : readOnlyFlag(false),
        declarations(std::vector<CSSStyleDeclaration*>()),
        parentRule(nullptr),
        ownerNode(nullptr){};
  ~CSSStyleDeclarationBlock() {
    delete parentRule;
    delete ownerNode;
  };
};

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
  std::vector<CSSRule*> items;
  unsigned long length;

 public:
  CSSRule* operator[](unsigned long index) { return items[index]; }
  CSSRuleList() : length(0){};
  ~CSSRuleList(){};
  inline CSSRule* item(unsigned long index) {
    if (index >= items.size())
      return nullptr;
    return items[index];
  }
  void insertRule(CSSRule* cssRule, unsigned long index) {
    length++;
    items[index] = cssRule;
  }
  void pushRule(CSSRule* cssRule, unsigned long index) {
    length++;
    items.push_back(cssRule);
  }
  inline unsigned int size() const { return items.size(); }
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
 private:
  CSSRule* ownerRule;
  CSSRuleList* cssRules;

 public:
  CSSStyleSheet();
  CSSStyleSheet(CSSRuleList* list);
  ~CSSStyleSheet();
  inline CSSRuleList* const getCSSRules() const { return cssRules; }
  inline CSSRule* const getOwnerRule() const { return ownerRule; }
  inline unsigned long insertRule(std::string rule, unsigned long index) {
    CSSRule* cssRule = CSSRuleFactory::createCSSRule(CSSRule::STYLE_RULE);
    cssRules->insertRule(cssRule, index);
    return index;
  };
  inline void setCSSRuleList(CSSRuleList* list) { cssRules = list; }
  // TODO: implement it
  void deleteRule(unsigned long index){};
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
  void insert(StyleSheet* styleSheet) {
    length++;
    items.push_back(styleSheet);
  }
};

}  // namespace CSS

}  // namespace Flash

#endif
