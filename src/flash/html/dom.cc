#include "dom.h"

namespace Flash {
namespace DOM {

std::string nodeType2str(NodeType type) {
  switch (type) {
    case ELEMENT_NODE: {
      return "ELEMENT_NODE";
    }
    case ATTRIBUTE_NODE: {
      return "ATTRIBUTE_NODE";
    }
    case TEXT_NODE: {
      return "TEXT_NODE";
    }
    case CDATA_SECTION_NODE: {
      return "CDATA_SECTION_NODE";
    }
    case ENTITY_REFERENCE_NODE: {
      return "ENTITY_REFERENCE_NODE";
    }
    case ENTITY_NODE: {
      return "ENTITY_NODE";
    }
    case PROCESSING_INSTRUCTION_NODE: {
      return "PROCESSING_INSTRUCTION_NODE";
    }
    case COMMENT_NODE: {
      return "COMMENT_NODE";
    }
    case DOCUMENT_NODE: {
      return "DOCUMENT_NODE";
    }
    case DOCUMENT_TYPE_NODE: {
      return "DOCUMENT_TYPE_NODE";
    }
    case DOCUMENT_FRAGMENT_NODE: {
      return "DOCUMENT_FRAGMENT_NODE";
    }
    case NOTATION_NODE: {
      return "NOTATION_NODE";
    }
    default: {
      return "INVALID_TYPE";
    }
  };
}
}  // namespace DOM
}  // namespace Flash
