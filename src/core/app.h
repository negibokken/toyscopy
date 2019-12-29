#ifndef APP_h
#define APP_h

#include <gtk/gtk.h>
#include <gtkmm.h>

#include <iostream>
#include "../flash/html/html_document_parser.h"
#include "../flash/html/renderer.h"

class ToyScopyApp : public Gtk::Window {
 public:
  ToyScopyApp();
  ~ToyScopyApp();

 private:
  std::string url;
  Gtk::Entry m_entry;
  void on_enter();
  Gtk::HeaderBar m_header_bar;

  Gtk::Label m_title;
  Gtk::ScrolledWindow *m_scrolled_window;

  void set_title(std::string title);

  void load();
};

#endif
