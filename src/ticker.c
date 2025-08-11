#include "ticker.h"
#include "api.h"
#include "glib.h"
#include <stdio.h>

gpointer ticker_fetch(const char* ticker){
    StockValue* stock = api_get_stock_value(ticker);
    if(stock){
        printf("Stock current information: \n-> Symbol: %s\n-> Currency: %s\n-> Regular Market Price: %f\n-> Previous Close: %f\n", 
               stock->symbol, stock->currency, stock->regularMarketPrice, stock->previousClose);
        api_free_stock_value(stock);
    }

    return nullptr;
}
