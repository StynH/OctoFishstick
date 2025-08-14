#include "window.h"
#include <curl/curl.h>
#include "app.h"

int main(int argc, char* argv[]){
    curl_global_init(CURL_GLOBAL_ALL);

    g_print("Starting...\n");
    AppCtx* context = malloc(sizeof(AppCtx));
    context->app = app_create();

    g_print("Creating window...\n");
    window_initialize(context);

    g_print("Starting app...\n");
    return app_run(context, argc, argv);
}
