#include "../flash/html/html_document_parser.h"
#include "../flash/html/renderer.h"

#include <gtk/gtk.h>
#include <stack>

static void print_hello(GtkWidget *widget, gpointer data)
{
  g_print("Hello World\n");
}

static void activate(GtkApplication *app, gpointer user_data)
{
  GtkWidget *window;
  GtkWidget *button;
  GtkWidget *button_box;

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "ToyScopy");
  gtk_window_set_default_size(GTK_WINDOW(window), 480, 480);
  gtk_window_set_resizable(GTK_WINDOW(window), true);
  gtk_window_set_gravity(GTK_WINDOW(window), GDK_GRAVITY_SOUTH_EAST);

  HTMLDocumentParser *hdp =
      new HTMLDocumentParser("<html><body>Hello World</body></html>");
  hdp->parse();

  // TODO: make CSSOM

  // Renderer
  Render::Renderer *r = new Render::Renderer(window, hdp->document, NULL);
  r->render();

  // button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);

  // gtk_container_add(GTK_CONTAINER(window), button_box);
  // gtk_widget_show_all(window);
}

int main(int argc, char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}
