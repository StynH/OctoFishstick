#include "api.h"
#include <curl/curl.h>
#include <stdlib.h>

int main(){
    curl_global_init(CURL_GLOBAL_ALL);

    StockValue* stock = api_get_stock_value("XAIX.DE");
    if(stock){
        printf("Stock current information: \n-> Symbol: %s\n-> Currency: %s\n-> Regular Market Price: %f\n-> Previous Close: %f\n", 
               stock->symbol, stock->currency, stock->regularMarketPrice, stock->previousClose);
        free(stock);
    }

    curl_global_cleanup();
    return 0;
}
