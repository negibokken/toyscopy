#ifndef DOM_H
#define DOM_H

#include <iostream>
#include <queue>
#include <string>
#include <vector>

namespace DOM {

class Element;
class Node;
enum NodeType {
  ELEMENT_NODE = 1,
  ATTRIBUTE_NODE = 2,
  TEXT_NODE = 3,
  CDATA_SECTION_NODE = 4,
  ENTITY_REFERENCE_NODE = 5,
  ENTITY_NODE = 6,
  PROCESSING_INSTRUCTION_NODE = 7,
  COMMENT_NODE = 8,
  DOCUMENT_NODE = 9,
  DOCUMENT_TYPE_NODE = 10,
  DOCUMENT_FRAGMENT_NODE = 11,
  NOTATION_NODE = 12,
};
class Node {
 public:
  unsigned short nodeType;
  std::string nodeName;

  Node(unsigned short t) { nodeType = t; }

  std::string baseURI;
  bool isConnected;
  Node *parentNode;
  Element *parentElement;
  std::vector<Node *> childNodes;

  Node appendChild(Node *node) {
    std::cout << "node type:" << node->nodeType << std::endl;
    this->childNodes.push_back(node);
    return *node;
  }
};

class Element : public Node {
 public:
  Element() : Node(NodeType::ELEMENT_NODE){};
  std::string tagName;
};

inline std::ostream &operator<<(std::ostream &os, Element &e) {
  os << "Element:" << std::endl;
  os << "  tagName: " << e.tagName << std::endl;
  os << "  elementType: " << e.nodeType << std::endl;
  return os;
};

enum DocumentReadyState { loading, interactive, complete };
class Document : public Node {
 public:
  Document() : Node(NodeType::DOCUMENT_NODE){};
  std::string URL;
  std::string DocumentURI;
  std::string origin;
  std::string characterSet;
  std::string charset;
  std::string contentType;

  DocumentReadyState readyState;
  Element documentElement;
  void printAllNode() {
    std::queue<Node *> q;
    q.push(this);
    while (!q.empty()) {
      Node *node = q.front();
      q.pop();
      std::cout << node << std::endl;
      for (auto n : node->childNodes) {
        std::cout << n << std::endl;
      }
    }
  }
};

class CharacterData : public Node {
 public:
  CharacterData() : Node(NodeType::TEXT_NODE){};
  std::string data;
  unsigned long length;
  void appendData(std::string data) { this->data.append(data); }
};

class Text : public CharacterData {
 public:
  Text(std::string txt) : CharacterData() { this->data = txt; };
  std::string wholeText() { return this->data; };
};

inline std::ostream &operator<<(std::ostream &os, Text &t) {
  os << t.wholeText();
  return os;
};

}  // namespace DOM
#endif
