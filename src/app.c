#include "app.h"
#include "gio/gio.h"
#include "glib-object.h"
#include "gtk/gtk.h"
#include <curl/curl.h>

static void app_cleanup(AppCtx* context);

GtkApplication* app_create(){
    return gtk_application_new("org.octofishstick", G_APPLICATION_DEFAULT_FLAGS);
}

int app_run(AppCtx* context, int argc, char* argv[]){
    int status = g_application_run(G_APPLICATION(context->app), argc, argv);
    g_object_unref(context->app);

    app_cleanup(context);
    return status;
}

static void app_cleanup(AppCtx* context){
    curl_global_cleanup();
    free(context);
}
