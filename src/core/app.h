#ifndef APP_h
#define APP_h

#include <gtk/gtk.h>

#include <iostream>
#include "../flash/html/html_document_parser.h"
#include "../flash/html/renderer.h"

class ToyScopyApp {
 public:
  int run();

 private:
  std::string url;
  GtkApplication* app;
  GtkWidget* window;

  void activate(GtkApplication* app, gpointer user_data);
  void attachHeaderBar();
  void load();
};

#endif
