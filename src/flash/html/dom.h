#ifndef DOM_H
#define DOM_H

#include <iostream>
#include <string>
#include <vector>

namespace DOM {

class Element;
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

  Node appendChild(Node *node)
  {
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

}  // namespace DOM
#endif
