#include "./app.h"

#include <gtkmm.h>

int main(int argc, char **argv) {
  Glib::RefPtr<Gtk::Application> app =
      Gtk::Application::create(argc, argv, "io.bokken");

  ToyScopyApp tsapp;

  return app->run(tsapp);

  // ToyScopyApp *app = new ToyScopyApp();
  // app->run();
  // return 0;
}
