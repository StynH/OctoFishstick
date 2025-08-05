#include "window.h"
#include "app.h"
#include <curl/curl.h>

int main(){
    curl_global_init(CURL_GLOBAL_ALL);

    GtkApplication* app = app_create();
    window_initialize(app);
    return app_run(app);
}
