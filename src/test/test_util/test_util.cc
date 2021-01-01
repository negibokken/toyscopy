#include <string>

#include "../../flash/html/dom.h"

std::string expandAttribute(Flash::DOM::Element* ele) {
  std::string result = "";
  std::vector<std::string> attrs = ele->getAttributeNames();
  if (attrs.size() == 0)
    return "";

  int cnt = 0;
  result += " ";
  for (auto it = attrs.rbegin(); it != attrs.rend(); it++) {
    std::string attr = *it;
    result += (attr + "=" + "\"" + ele->getAttribute(attr) + "\"");
    if (cnt < attrs.size() - 1) {
      result += " ";
    }
    cnt++;
  }
  return result;
}

std::string expandElement(Flash::DOM::Element* ele) {
  std::string result = "";
  result += ("<" + ele->getTagName() + expandAttribute(ele) +
             (ele->getTagName() == "meta" ? "/" : "") + ">");
  return result;
}

std::string dfs(Flash::DOM::Node* node, Flash::DOM::Node* head) {
  if (node == NULL || node == nullptr)
    return "";
  std::string str = "";

  // If the type of node is Element put <element>
  if (node->nodeType == Flash::DOM::NodeType::ELEMENT_NODE) {
    Flash::DOM::Element* ele = static_cast<Flash::DOM::Element*>(node);
    str += (expandElement(ele));
  } else if (node->nodeType == Flash::DOM::NodeType::TEXT_NODE) {
    str += (reinterpret_cast<Flash::DOM::Text*>(node)->getData());
  } else if (node->nodeType == Flash::DOM::NodeType::DOCUMENT_TYPE_NODE) {
    Flash::DOM::DocumentType* ele =
        static_cast<Flash::DOM::DocumentType*>(node);
    str += "<!doctype " + ele->name + ">";
  }

  if (node->nodeType == Flash::DOM::NodeType::ELEMENT_NODE) {
    Flash::DOM::Element* ele = static_cast<Flash::DOM::Element*>(node);
    if (ele->getTagName() == "html" && head->childNodes.size() != 0) {
      str += dfs(head, head);
    }
  }

  for (auto n : node->childNodes) {
    str += dfs(n, head);
  }

  // If the type of Flash::DOM is Element, put close tag. e.g. </element>
  if (node->nodeType == Flash::DOM::NodeType::ELEMENT_NODE) {
    Flash::DOM::Element* ele = static_cast<Flash::DOM::Element*>(node);
    if (ele->getTagName() == "meta")
      return str;

    str += ("</" + reinterpret_cast<Flash::DOM::Element*>(node)->getTagName() +
            ">");
  }
  return str;
}

std::string dom2string(Flash::DOM::Node* node, Flash::DOM::Node* head) {
  std::string txt = dfs(node, head);
  return txt;
}