#include <gtk/gtk.h>
#include "src/resources.h"

#define CSS_FILE_NAME "./resources/app.css"

void resources_load_css(){
    GtkCssProvider *provider = gtk_css_provider_new();

    GFile* file = g_file_new_for_path(CSS_FILE_NAME);
    gtk_css_provider_load_from_file(provider, file);
    g_object_unref(file);

    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );

    g_object_unref(provider);
}
