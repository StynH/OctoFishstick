#include "api.h"
#include <curl/curl.h>
#include <stdio.h>

int main(){
    curl_global_init(CURL_GLOBAL_ALL);

    api_get_stock_value("XAIX.DE");

    curl_global_cleanup();
    return 0;
}
