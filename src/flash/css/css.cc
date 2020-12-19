#include "css.h"

namespace Flash {
namespace CSS {

MediaList::MediaList() {}
MediaList::~MediaList() {}
std::string MediaList::item(unsigned long index) {
  if (index > items.size() - 1) {
    return "";
  }
  return items[index];
}
void MediaList::appendMedium(std::string medium) {
  items.push_back(medium);
  length++;
}
void MediaList::deleteMedium(std::string medium) {
  for (auto it = items.begin(); it != items.end(); ++it) {
    if (*it == medium) {
      items.erase(it);
      return;
    }
  }
}

StyleSheet::StyleSheet() {}

// CSSStyleSheet
CSSStyleSheet::CSSStyleSheet(CSSRuleList* list) {
  ownerRule = new CSSRule(CSSRule::STYLE_RULE);
  cssRules = list;
}
StyleSheet::~StyleSheet() {}

CSSStyleSheet::CSSStyleSheet() {
  ownerRule = new CSSRule(CSSRule::STYLE_RULE);
  cssRules = new CSSRuleList();
}
CSSStyleSheet::~CSSStyleSheet() {
  delete ownerRule;
  delete cssRules;
}

CSSStyleRule::~CSSStyleRule() {
  delete declaration;
}

void CSSStyleRule::appendDeclarations(CSSStyleDeclaration* dec) {
  const std::vector<std::string> keys = dec->getPropertyKeys();
  for (const auto& key : keys) {
    std::string val = dec->getPropertyValue(key);
    std::string priority = dec->getPropertyPriority(key);
    declaration->setProperty(key, val, priority);
  }
}

}  // namespace CSS
}  // namespace Flash