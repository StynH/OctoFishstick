#include "window.h"
#include <curl/curl.h>
#include <stdio.h>
#include "app.h"

int main(int argc, char* argv[]){
    curl_global_init(CURL_GLOBAL_ALL);

    printf("Starting...\n");

    AppCtx* context = malloc(sizeof(AppCtx));
    context->app = app_create();

    printf("Creating window...\n");
    window_initialize(context);

    printf("Starting app...\n");
    return app_run(context, argc, argv);
}
