#ifndef DOM_H
#define DOM_H

#include <iostream>
#include <queue>
#include <string>
#include <vector>

namespace DOM {

class Element;
class Node;
class DOMImplementation;
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

class DocumentType : public Node {
 public:
  std::string name;
  std::string publicId;
  std::string systemId;
  DocumentType(std::string name, std::string publicId, std::string systemId)
      : name(name),
        publicId(publicId),
        systemId(systemId),
        Node(NodeType::DOCUMENT_TYPE_NODE){};
};

class Element : public Node {
 public:
  Element(std::string tagName) : Node(NodeType::ELEMENT_NODE) {
    setTagName(tagName);
  };
  void setTagName(std::string str) { tagName = str; }
  std::string getTagName() const { return tagName; }
  std::string tagName;
};

inline std::ostream &operator<<(std::ostream &os, Element &e) {
  os << "Element:" << std::endl;
  os << "  tagName: " << e.tagName << std::endl;
  os << "  elementType: " << e.nodeType << std::endl;
  return os;
};

class CharacterData : public Node {
 public:
  std::string data;
  CharacterData() : Node(NodeType::TEXT_NODE) { data = ""; };
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

class DOMImplementation {
 public:
  DocumentType *createDocumentType(std::string qualifiedName,
                                   std::string publicId, std::string systemId) {
    return new DocumentType(qualifiedName, publicId, systemId);
  }
};

enum DocumentReadyState { loading, interactive, complete };
class Document : public Node {
 public:
  DOMImplementation *implementation;
  Document() : Node(NodeType::DOCUMENT_NODE){};
  std::string URL;
  std::string DocumentURI;
  std::string origin;
  std::string characterSet;
  std::string charset;
  std::string contentType;

  DocumentReadyState readyState;
  // Element documentElement;
  Element *createElement(std::string name) { return new Element(name); };
  Text *createText(std::string data) { return new Text(data); };
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

}  // namespace DOM
#endif
