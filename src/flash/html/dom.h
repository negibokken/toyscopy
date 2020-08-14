#ifndef DOM_H
#define DOM_H

#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

#include "../../utils/log_util.h"
#include "../css/css.h"

namespace Flash {

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
  std::string nodeName = "";

  Node(unsigned short t) { nodeType = t; }

  std::string baseURI = "";
  bool isConnected;
  Node* parentNode;
  Element* parentElement;
  std::vector<Node*> childNodes;

  virtual Node appendChild(Node* node) {
    ToyScopyUtil::logUtil("node type: %d", node->nodeType);
    node->parentNode = this;

    this->isConnected = true;
    node->isConnected = true;

    this->childNodes.push_back(node);
    return *node;
  }

  inline Node* firstChild() {
    if (childNodes.empty()) {
      return NULL;
    }
    return childNodes.front();
  }
  inline Node* lastChild() {
    if (childNodes.empty()) {
      return NULL;
    }
    return childNodes.back();
  }
};

class DocumentType : public Node {
 public:
  std::string name = "";
  std::string publicId = "";
  std::string systemId = "";
  DocumentType(std::string name, std::string publicId, std::string systemId)
      : name(name),
        publicId(publicId),
        systemId(systemId),
        Node(NodeType::DOCUMENT_TYPE_NODE){};
};

class Element : public Node {
 private:
  std::string tagName = "";
  std::map<std::string, std::string> attributes;
  void setTagName(std::string str) { tagName = str; }

 public:
  Element(std::string tagName) : Node(NodeType::ELEMENT_NODE) {
    setTagName(tagName);
  };
  std::string getTagName() const { return tagName; }
  inline std::string getAttribute(std::string attrName) {
    return attributes[attrName];
  }
  inline void setAttribute(std::string attrName, std::string attrValue) {
    attributes[attrName] = attrValue;
  }
  std::vector<std::string> getAttributeNames() {
    std::vector<std::string> names;
    for (auto attribute : attributes) {
      names.push_back(attribute.first);
    }
    return names;
  };
  virtual Node appendChild(Node* node) {
    ToyScopyUtil::logUtil("node type: %d", node->nodeType);
    Node::appendChild(node);
    node->parentElement = this;
    return *node;
  }
};

inline std::ostream& operator<<(std::ostream& os, Element& e) {
  os << "Element:" << std::endl;
  os << "  tagName: " << e.getTagName() << std::endl;
  os << "  elementType: " << e.nodeType << std::endl;
  return os;
};

class CharacterData : public Node {
  unsigned long length;
  std::string data = "";

 public:
  CharacterData(std::string txt) : Node(NodeType::TEXT_NODE) { data = txt; };
  inline std::string getData() { return data; }
  inline void setData(std::string data) { data = data; }
  void appendData(std::string data) { this->data.append(data); }
};

class Text : public CharacterData {
 public:
  Text(std::string txt) : CharacterData(txt){};
  std::string wholeText() { return getData(); };
};

inline std::ostream& operator<<(std::ostream& os, Text& t) {
  os << t.wholeText();
  return os;
};

class DOMImplementation {
 public:
  DocumentType* createDocumentType(std::string qualifiedName,
                                   std::string publicId,
                                   std::string systemId) {
    return new DocumentType(qualifiedName, publicId, systemId);
  }
};

enum DocumentReadyState { loading, interactive, complete };

class Document : public Node {
 private:
  CSS::StyleSheetList* styleSheets;

 public:
  DOMImplementation* implementation;
  Document() : Node(NodeType::DOCUMENT_NODE){};
  std::string URL = "";
  std::string DocumentURI = "";
  std::string origin = "";
  std::string characterSet = "";
  std::string charset = "";
  std::string contentType = "";

  DocumentReadyState readyState;
  Element* createElement(std::string name) { return new Element(name); };
  Text* createText(std::string data) { return new Text(data); };
};

}  // namespace DOM
}  // namespace Flash
#endif
