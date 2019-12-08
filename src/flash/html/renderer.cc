#include "renderer.h"

namespace Render {

RenderObject *RenderObject::createObject(DOM::Node *node, RenderStyle *style) {
  CSS::Style s = CSS::Style::INLINE;

  RenderObject *o = NULL;
  switch (s) {
    case CSS::Style::INLINE: {
      RenderInline *r = new RenderInline(node);
      o = reinterpret_cast<RenderObject *>(r);
      break;
    }
    default: { break; }
  }

  return o;
};

void RenderInline::layout() {
  // std::string str;
  // if (this->node->nodeType == DOM::NodeType::TEXT_NODE) {

  // std::cout << "kokoni" << std::endl;
  // std::cout << this->node->nodeType << std::endl;
  // std::cout << reinterpret_cast<DOM::Text *>(this->node)->data << std::endl;
  // std::cout << "kokoni" << std::endl;
  // const gchar *str = reinterpret_cast<DOM::Text *>(this->node)->data.c_str();
  int itr = 0;
  std::string dom_str = reinterpret_cast<DOM::Text *>(this->node)->data;
  const int MAX_CHAR = 4096;
  char data[MAX_CHAR];
  while (dom_str[itr] != '\0' || itr > MAX_CHAR - 1) {
    data[itr] = dom_str[itr];
    itr++;
  }
  data[itr] = '\0';
  const gchar *str = data;

  std::cout << str << std::endl;

  GtkWidget *label;
  label = gtk_label_new(str);
  gtk_label_set_selectable(GTK_LABEL(label), TRUE);
  this->widget = label;
}

void RenderInline::paint() {
  gtk_container_add(GTK_CONTAINER(this->container), this->widget);
}

Renderer::Renderer(GtkWidget *window, DOM::Document *dom, CSS::CSSOM *cssom)
    : window(window), dom(dom), cssom(cssom) {}

void Renderer::render() {
  std::cout << "process-render" << std::endl;
  GtkWidget *container;
  container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

  std::stack<DOM::Node *> st;
  st.push(dom);
  while (!st.empty()) {
    DOM::Node *node = st.top();
    st.pop();
    if (node->childNodes.size() != 0) {
      for (int i = node->childNodes.size() - 1; i >= 0; i--) {
        st.push(node->childNodes[i]);
      }
    } else {
      RenderObject *ro = RenderObject::createObject(node, NULL);

      std::cout << "=======" << std::endl;
      std::cout << "nodeType: " << node->nodeType << std::endl;
      std::cout << "textnode: " << reinterpret_cast<DOM::Text *>(node)->data
                << std::endl;

      // Process CSS Style
      CSS::Style s;
      if (node->nodeType == DOM::NodeType::TEXT_NODE) {
        s = CSS::Style::INLINE;
      } else if (node->nodeType == DOM::NodeType::DOCUMENT_NODE) {
        s = CSS::Style::BLOCK;
      } else {
        s = CSS::Style::BLOCK;
      }

      switch (s) {
        case CSS::Style::BLOCK: {
          break;
        }
        case CSS::Style::INLINE: {
          std::cout << node->nodeType << std::endl;
          RenderInline *ele = reinterpret_cast<RenderInline *>(ro);
          ele->container = container;
          ele->layout();
          ele->paint();
          break;
        }
        default: { break; }
      }
    }

    gtk_container_add(GTK_CONTAINER(this->window), container);
    gtk_widget_show_all(this->window);
  }
}

}  // namespace Render
