#ifndef OCTO_FISHSTICK_WIDGET_HELPERS
#define OCTO_FISHSTICK_WIDGET_HELPERS

#include <gtk/gtk.h>
#include <glib.h>

GtkWidget* ui_find(GtkWidget* root, const char* name);
GtkWidget* ui_find_nth(GtkWidget* root, const char* name, size_t index);
GPtrArray* ui_find_all(GtkWidget* root, const char* name);

#endif
