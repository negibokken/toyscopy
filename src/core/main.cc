#include <gtk/gtk.h>

#include <stack>

#include "../flash/html/html_document_parser.h"
#include "../flash/html/renderer.h"

static void attachHeaderBar(GtkWidget *window) {
  GtkWidget *header_bar;
  header_bar = gtk_header_bar_new();
  gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(header_bar), true);

  GtkWidget *search_bar;
  search_bar = gtk_entry_new();
  gtk_entry_set_width_chars(GTK_ENTRY(search_bar), 50);
  gtk_entry_set_placeholder_text(GTK_ENTRY(search_bar), "URL");
  gtk_header_bar_set_custom_title(GTK_HEADER_BAR(header_bar), search_bar);
  gtk_window_set_titlebar(GTK_WINDOW(window), header_bar);

  gtk_widget_show_all(window);
}

static void activate(GtkApplication *app, gpointer user_data) {
  GtkWidget *window;

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "toyscopy");
  gtk_window_set_default_size(GTK_WINDOW(window), 480, 480);
  gtk_window_set_resizable(GTK_WINDOW(window), true);
  gtk_window_set_gravity(GTK_WINDOW(window), GDK_GRAVITY_SOUTH_EAST);
  attachHeaderBar(window);

  // Call HTML Renderer
  HTMLDocumentParser *hdp = new HTMLDocumentParser(
      "<html><head></head><body>Hello World</body></html>");
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
      std::cout << cur->childNodes.size() << std::endl;
      for (auto child : cur->childNodes) {
        std::cout << child->nodeName << "," << nodetype[child->nodeType - 1]
                  << std::endl;
        q.push(child);
      }
    }
  }
  std::cout << "=== analyzed ===" << std::endl;

  // TODO: make CSSOM

  // Renderer
  Render::Renderer *r = new Render::Renderer(window, hdp->document, NULL);
  r->render();
}

int main(int argc, char **argv) {
  GtkApplication *app;
  int status;

  app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}
