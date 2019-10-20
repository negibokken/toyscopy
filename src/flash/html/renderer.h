#ifndef Render_h
#define Render_h

#include "css.h"
#include "dom.h"
#include "gtk/gtk.h"

#include <iostream>
#include <stack>

namespace Render {

class Renderer;
class RenderObject;
class RenderStyle;
class RenderInline;

class RenderObject {
  public:
  DOM::Node *node;
  RenderObject(DOM::Node *node) : node(node) {}
  virtual void layout() {}
  virtual void paint() {}
  static RenderObject *createObject(DOM::Node *node, RenderStyle *style);
};

class RenderStyle {
  public:
  RenderStyle(){};
};

class RenderInline : public RenderObject {
  public:
  GtkWidget *container;
  GtkWidget *widget;
  RenderInline(DOM::Node *node) : RenderObject(node){};
  void layout();
  void paint();
};

class Renderer {
  public:
  GtkWidget *window;
  DOM::Document *dom;
  CSS::CSSOM *cssom;
  Renderer(GtkWidget *window, DOM::Document *dom, CSS::CSSOM *cssom);
  void render();
};

}  // namespace Render
#endif
