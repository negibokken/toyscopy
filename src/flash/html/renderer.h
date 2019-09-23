#ifndef Layout_h
#define Layout_h

#include "css.h"
#include "dom.h"

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
  RenderObject() {}
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
  RenderInline(){};
  void layout();
  void paint();
};

class Renderer {
  public:
  DOM::Document *dom;
  CSS::CSSOM *cssom;
  Renderer(DOM::Document *dom, CSS::CSSOM *cssom);
  void render();
};

}  // namespace Render
#endif
