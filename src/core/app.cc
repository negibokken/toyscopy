#include "app.h"

ToyScopyApp::ToyScopyApp() {
  this->set_title("toyscopy");
  this->set_titlebar(m_header_bar);
  this->set_default_size(500, 500);
  this->property_gravity().set_value(Gdk::Gravity::GRAVITY_NORTH_EAST);

  // Entry
  m_entry.set_width_chars(50);
  m_entry.set_placeholder_text("URL");
  m_entry.signal_activate().connect(
      sigc::mem_fun(*this, &ToyScopyApp::on_enter));

  m_header_bar.set_custom_title(m_entry);
  m_header_bar.set_show_close_button(true);
  m_header_bar.show_all();

  m_scrolled_window = new Gtk::ScrolledWindow();
  // scroll window
  add(*m_scrolled_window);
  this->load();
  // m_scrolled_window->show_all();
}

ToyScopyApp::~ToyScopyApp() {}

void ToyScopyApp::on_enter() { std::cout << m_entry.get_text() << std::endl; }

void ToyScopyApp::load() {
  // Call HTML Renderer
  HTMLDocumentParser *hdp = new HTMLDocumentParser(
      "<!DOCTYPE html><html><head></head><body><div><h1>Hello "
      "Title</h1><p>Hello "
      "World</p></div></body></html>");
  hdp->parse();

  // node name node type
  std::string nodetype[] = {
      "ELEMENT_NODE",
      "ATTRIBUTE_NODE",
      "TEXT_NODE",
      "CDATA_SECTION_NODE",
      "ENTITY_REFERENCE_NODE",
      "ENTITY_NODE",
      "PROCESSING_INSTRUCTION_NODE",
      "COMMENT_NODE",
      "DOCUMENT_NODE",
      "DOCUMENT_TYPE_NODE",
      "DOCUMENT_FRAGMENT_NODE",
      "NOTATION_NODE",
  };

  std::cout << "=== parsed ===" << std::endl;
  // traverse
  {
    DOM::Node *cur;
    std::queue<DOM::Node *> q;
    q.push(hdp->document);
    while (!q.empty()) {
      cur = q.front();
      q.pop();
      switch (cur->nodeType) {
        case DOM::ELEMENT_NODE: {
          DOM::Element *element = static_cast<DOM::Element *>(cur);
          std::cout << "tag: " << element->tagName << std::endl;
        }
        case DOM::TEXT_NODE: {
          DOM::Text *textnode = static_cast<DOM::Text *>(cur);
          std::cout << "text: " << textnode->wholeText() << std::endl;
          break;
        }
        case DOM::DOCUMENT_NODE: {
          std::cout << "Document" << std::endl;
          break;
        }
        default:
          break;
      }
      std::cout << "child size: " << cur->childNodes.size() << std::endl;
      for (auto child : cur->childNodes) {
        q.push(child);
      }
      std::cout << std::endl;
    }
  }
  std::cout << "=== analyzed ===" << std::endl;
  // TODO: make CSSOM

  // Renderer
  Render::Renderer *r =
      new Render::Renderer(m_scrolled_window, hdp->document, NULL);
  r->render();
}
