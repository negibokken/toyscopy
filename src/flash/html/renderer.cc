#include "renderer.h"

namespace Render {

RenderObject *RenderObject::createObject(DOM::Node *node, RenderStyle *style)
{
  CSS::Style s = CSS::Style::INLINE;

  RenderObject *o = NULL;
  switch (s) {
    case CSS::Style::INLINE: {
      RenderInline *r = new RenderInline();
      o = reinterpret_cast<RenderObject *>(r);
      break;
    }
    default: {
      break;
    }
  }

  return o;
};

void RenderInline::layout() { std::cout << "text_layout" << std::endl; }
void RenderInline::paint() { std::cout << "text_print" << std::endl; }

Renderer::Renderer(DOM::Document *dom, CSS::CSSOM *cssom)
    : dom(dom), cssom(cssom)
{
}

void Renderer::render()
{
  std::stack<DOM::Node *> st;
  st.push(dom);
  while (!st.empty()) {
    DOM::Node *node = st.top();
    st.pop();
    if (node->childNodes.size() != 0) {
      for (int i = node->childNodes.size() - 1; i >= 0; i--) {
        st.push(node->childNodes[i]);
      }
    }
    else {
      RenderObject *ro = RenderObject::createObject(node, NULL);

      CSS::Style s = CSS::Style::INLINE;
      switch (s) {
        case CSS::Style::INLINE: {
          RenderInline *ele = reinterpret_cast<RenderInline *>(ro);
          ele->layout();
          ele->paint();
          break;
        }
        default: {
          break;
        }
      }
    }
  }
}

}  // namespace Render
