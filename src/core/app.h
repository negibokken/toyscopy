#ifndef APP_h
#define APP_h

#include <gtk/gtk.h>
#include <gtkmm.h>

#include <iostream>

#include "../flash/html/html_document_parser.h"
#include "../flash/html/renderer.h"
#include "../utils/log_util.h"
#include "../utils/simple_http_client.h"

class ToyScopyApp : public Gtk::Window {
 public:
  ToyScopyApp();
  ~ToyScopyApp();

 private:
  std::string url = "";
  Gtk::Entry m_entry;
  void on_enter();
  Gtk::HeaderBar m_header_bar;

  Gtk::Label m_title;
  Gtk::ScrolledWindow* m_scrolled_window;

  ToyScopyUtil::SimpleHttpClient* httpclient;

  void set_title(std::string title);
  void set_url(std::string url);

  std::string src = "";
  const std::string defaultSrc =
      "<!doctype html><html><head>"
      "<meta charset=\"utf-8\" />"
      "<meta http-equiv=\"Content-type\" content=\"text/html; charset=utf-8\""
      "/>"
      "<meta name=\"viewport\" content=\"width=device-width,"
      "initial-scale=1\" "
      "/>"
      "<title>About ToyScopy</title>"
      "<style type=\"text/css\">"
      "    body {"
      "        background-color: #f0f0f2;"
      "        margin: 0;"
      "        padding: 0;"
      "    }"
      "    div {"
      "        width: 600px;"
      "        margin: 5em auto;"
      "        padding: 2em;"
      "        background-color: #fdfdff;"
      "        border-radius: 0.5em;"
      "        box-shadow: 2px 3px 7px 2px rgba(0,0,0,0.02);"
      "    }"
      "</style>"
      "</head><body><div>"
      "<h1>ToyScopy</h1>"
      "<p>A tiny browser written in C++.</p>"
      "<p><a href=\"https://github.com/negibokken/toyscopy\">"
      "For more information, see here: https://github.com/negibokken/toyscopy"
      "</a></p></div></body></html>";
  void load();
};

#endif
