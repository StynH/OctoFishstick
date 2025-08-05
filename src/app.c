#include "gio/gio.h"
#include "glib-object.h"
#include "gtk/gtk.h"
#include <curl/curl.h>

void app_cleanup();

GtkApplication* app_create(){
    return gtk_application_new("org.octofishstick", G_APPLICATION_DEFAULT_FLAGS);
}

int app_run(GtkApplication* app){
    int status = g_application_run(G_APPLICATION(app), 0, nullptr);
    g_object_ref(app);

    app_cleanup();
    return status;
}

void app_cleanup(){
    curl_global_cleanup();
}
