#include "renderer.h"

namespace Flash {
namespace Render {

RenderObject* RenderObject::createObject(DOM::Node* node, RenderStyle* style) {
  CSS::Style s = CSS::Style::INLINE;

  RenderObject* o = NULL;
  switch (s) {
    case CSS::Style::INLINE: {
      RenderInline* r = new RenderInline(node);
      o = reinterpret_cast<RenderObject*>(r);
      break;
    }
    default: {
      break;
    }
  }

  return o;
};

void RenderInline::layout() {
  int itr = 0;
  std::string dom_str = reinterpret_cast<DOM::Text*>(this->node)->getData();
  const int MAX_CHAR = 4096;
  char data[MAX_CHAR];
  while (dom_str[itr] != '\0' || itr > MAX_CHAR - 1) {
    data[itr] = dom_str[itr];
    itr++;
  }
  data[itr] = '\0';
  const gchar* str = data;

  ToyScopyUtil::logUtil("%s", str);

  Gtk::Label* label = new Gtk::Label();
  label->set_text(str);
  label->set_selectable(true);
  label->set_xalign(0.0);
  label->set_line_wrap(true);
  label->override_color(Gdk::RGBA("#ff0000"));
  widget = label;
}

void RenderInline::paint() {
  // Gdk::RGBA color = Gdk::RGBA("#ff0000");
  // widget->override_background_color(color);
  container->add(*widget);
}

Renderer::Renderer(Gtk::ScrolledWindow* window,
                   DOM::Document* dom,
                   CSS::CSSOM* cssom)
    : window(window), dom(dom), cssom(cssom) {}

void Renderer::render() {
  ToyScopyUtil::logUtil("process-render");
  Gtk::Container* container =
      (new Gtk::Box(Gtk::Orientation::ORIENTATION_VERTICAL, 0));
  ToyScopyUtil::logUtil("----");
  // container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

  std::stack<DOM::Node*> st;
  st.push(dom);
  while (!st.empty()) {
    DOM::Node* node = st.top();
    st.pop();
    if (node->childNodes.size() != 0) {
      for (int i = node->childNodes.size() - 1; i >= 0; i--) {
        st.push(node->childNodes[i]);
      }
    }

    RenderObject* ro = RenderObject::createObject(node, NULL);

    ToyScopyUtil::logUtil("======");
    ToyScopyUtil::logUtil("nodeType: %d", node->nodeType);
    ToyScopyUtil::logUtil(
        "textnode: %s", reinterpret_cast<DOM::Text*>(node)->getData().c_str());

    // Process CSS Style
    CSS::Style s;
    if (node->nodeType == DOM::NodeType::TEXT_NODE) {
      s = CSS::Style::INLINE;
    } else if (node->nodeType == DOM::NodeType::DOCUMENT_NODE) {
      s = CSS::Style::BLOCK;
    } else if (node->nodeType == DOM::NodeType::ELEMENT_NODE) {
      s = CSS::Style::BLOCK;
      DOM::Element* ele = static_cast<DOM::Element*>(node);
      ToyScopyUtil::logUtil("%s", ele->getTagName().c_str());
    } else {
      s = CSS::Style::BLOCK;
    }

    switch (s) {
      case CSS::Style::BLOCK: {
        break;
      }
      case CSS::Style::INLINE: {
        ToyScopyUtil::logUtil("%d", node->nodeType);
        RenderInline* ele = reinterpret_cast<RenderInline*>(ro);
        ele->container = container;
        ele->layout();
        ele->paint();
        break;
      }
      default: {
        break;
      }
    }
  }
  this->window->add(*container);
  this->window->show_all();
}

}  // namespace Render
}  // namespace Flash
