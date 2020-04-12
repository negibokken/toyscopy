#include <string>

#include "../../flash/html/dom.h"

std::string dfs(DOM::Node *node) {
  std::string str = "";

  // If Element <element>
  if (node->nodeType == DOM::NodeType::ELEMENT_NODE) {
    // std::cout << "element_translation" << std::endl;
    str += ("<" + reinterpret_cast<DOM::Element *>(node)->getTagName() + ">");
  } else if (node->nodeType == DOM::NodeType::TEXT_NODE) {
    // std::cout << "text_translation" << std::endl;
    str += (reinterpret_cast<DOM::Text *>(node)->getData());
  }

  for (auto n : node->childNodes) {
    str += dfs(n);
  }

  // If Element </element>
  if (node->nodeType == DOM::NodeType::ELEMENT_NODE) {
    // std::cout << "end_tag_translation" << std::endl;
    str += ("</" + reinterpret_cast<DOM::Element *>(node)->getTagName() + ">");
  }
  return str;
}

std::string dom2string(DOM::Node *node) {
  std::string txt = dfs(node);
  return txt;
}