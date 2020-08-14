#include "app.h"

ToyScopyApp::ToyScopyApp() {
  // 初期化
  // TODO: 実際にはここは各種オブジェクトを渡す方がよさそう
  httpclient = new ToyScopyUtil::SimpleHttpClient();
  cssTokenizer = new Flash::CSSTokenizer();

  this->set_title("toyscopy");
  this->set_titlebar(m_header_bar);
  this->set_default_size(500, 500);
  this->property_gravity().set_value(Gdk::Gravity::GRAVITY_NORTH_EAST);

  // Entry
  m_entry.set_width_chars(50);
  m_entry.set_placeholder_text("URL");
  m_entry.signal_activate().connect(
      sigc::mem_fun(*this, &ToyScopyApp::on_enter));

  // Initialize title bar
  m_title.set_text("toyscopy");

  m_header_bar.set_custom_title(m_entry);
  m_header_bar.add(m_title);
  m_header_bar.set_show_close_button(true);
  m_header_bar.show_all();
  // Scroll Window
  m_scrolled_window = new Gtk::ScrolledWindow();
  add(*m_scrolled_window);
  this->load();
}

ToyScopyApp::~ToyScopyApp() {
  delete httpclient;
  delete m_scrolled_window;
  delete cssTokenizer;
}

void ToyScopyApp::on_enter() {
  this->remove();
  m_scrolled_window = new Gtk::ScrolledWindow();
  add(*m_scrolled_window);
  ToyScopyUtil::logUtil("url: %s", m_entry.get_text().c_str());
  const std::string urlString = m_entry.get_text();
  set_url(urlString);
  src = httpclient->fetch(urlString);
  src.erase(std::remove(src.begin(), src.end(), '\n'), src.end());
  std::string s = "";
  for (auto i = src.begin(); i != src.end(); i++) {
    s += *i;
    int cnt = 0;
    while (*(i + cnt) == ' ' && *(i + cnt + 1) == ' ') {
      cnt++;
    }
    i += cnt;
  }
  src = s;
  load();
}

void ToyScopyApp::set_title(std::string title) {
  m_title.set_text(title);
}
void ToyScopyApp::set_url(std::string _url) {
  url = _url;
}

void ToyScopyApp::load() {
  // Call HTML Renderer
  if (src.empty())
    src = defaultSrc;
  Flash::HTMLDocumentParser* hdp = new Flash::HTMLDocumentParser(src);
  ToyScopyUtil::logUtil("start parsing");
  hdp->parse();
  ToyScopyUtil::logUtil("finished parsing");

  set_title(hdp->getDocumentTitle());

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

  ToyScopyUtil::logUtil("=== header ===");
  for (auto n : hdp->head_pointer->childNodes) {
    DOM::Element* ele = static_cast<DOM::Element*>(n);
    ToyScopyUtil::logUtil("%s", ele->getTagName().c_str());
    std::cout << ele->getTagName() << std::endl;
    std::vector<std::string> attributeNames = ele->getAttributeNames();
    ToyScopyUtil::logUtil("attr: %d", ele->getAttributeNames().size());
    std::cout << ele->getAttributeNames().size() << std::endl;
    for (auto attr : attributeNames) {
      ToyScopyUtil::logUtil("%s = %s", attr.c_str(),
                            ele->getAttribute(attr).c_str());
      std::cout << ele->getAttribute(attr) << std::endl;
    }
    ToyScopyUtil::logUtil("child: %d", ele->childNodes.size());
    for (auto child : ele->childNodes) {
      DOM::Text* t = static_cast<DOM::Text*>(child);
      ToyScopyUtil::logUtil("child_content: ", t->getData().c_str());
      std::cout << t->getData() << std::endl;
    }
  }
  ToyScopyUtil::logUtil("=== body ===");
  // traverse
  {
    DOM::Node* cur;
    std::stack<DOM::Node*> q;
    q.push(hdp->document);
    while (!q.empty()) {
      cur = q.top();
      q.pop();
      switch (cur->nodeType) {
        case DOM::ELEMENT_NODE: {
          DOM::Element* element = static_cast<DOM::Element*>(cur);
          ToyScopyUtil::logUtil("tag: ", element->getTagName().c_str());
        }
        case DOM::TEXT_NODE: {
          DOM::Text* textnode = static_cast<DOM::Text*>(cur);
          ToyScopyUtil::logUtil("text: ", textnode->wholeText().c_str());
          break;
        }
        case DOM::DOCUMENT_NODE: {
          ToyScopyUtil::logUtil("Document");
          break;
        }
        default:
          break;
      }
      ToyScopyUtil::logUtil("child size: %d", cur->childNodes.size());
      std::vector<DOM::Node*> children = cur->childNodes;
      for (auto i = children.rbegin(); i != children.rend(); i++) {
        q.push(*i);
      }
      ToyScopyUtil::logUtil("---");
    }
  }
  ToyScopyUtil::logUtil("=== analyzed ===");
  // TODO: make CSSOM

  // Renderer
  Render::Renderer* r =
      new Render::Renderer(m_scrolled_window, hdp->document, NULL);
  r->render();
  ToyScopyUtil::logUtil("=== finish rendering ===");
  m_scrolled_window->show_all();
}
