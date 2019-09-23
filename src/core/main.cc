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

  HTMLDocumentParser *hdp = new HTMLDocumentParser("<html>helloworld</html>");
  hdp->parse();

  // TODO: make CSSOM

  Render::Renderer *r = new Render::Renderer(hdp->document, NULL);

  // Render
  Render::Renderer *renderer = new Render::Renderer(hdp->document, NULL);

  button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
  gtk_container_add(GTK_CONTAINER(window), button_box);

  GtkWidget *button2;

  button = gtk_button_new_with_label("Hello World");
  button2 = gtk_button_new_with_label("Hello World2");

  g_signal_connect(button2, "clicked", G_CALLBACK(print_hello), NULL);
  g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_widget_destroy),
                           window);

  GtkWidget *label;
  label = gtk_label_new("Hello World");
  gtk_label_set_selectable(GTK_LABEL(label), TRUE);

  // buffer = gtk_label_get_buffer(GTK_LABEL(label));

  // gtk_label_set_text(GTK_LABEL(label), "Hello, this is some text");

  gtk_container_add(GTK_CONTAINER(button_box), label);
  gtk_container_add(GTK_CONTAINER(button_box), button);
  gtk_container_add(GTK_CONTAINER(button_box), button2);

  gtk_widget_show_all(window);
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
