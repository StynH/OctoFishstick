#include <gtk/gtk.h>
#include "src/resources.h"
#include "gtk/gtkcssprovider.h"

#define CSS_RESOURCE_NAME "/com/octofishstick/css/app.css"

void resources_load_css(){
    GtkCssProvider *provider = gtk_css_provider_new();

    gtk_css_provider_load_from_resource(provider, CSS_RESOURCE_NAME);

    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );

    g_object_unref(provider);
}
