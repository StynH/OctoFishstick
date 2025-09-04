#include "api_worker.h"
#include "glib.h"
#include "src/api.h"
#include "src/app.h"
#include "src/views/watchlist.h"

gboolean api_worker_fetch_tickers(gpointer data){
    AppCtx* context = (AppCtx*)data;
    User* user = context->user;
    size_t watchlist_length = user_watchlist_length(user);

    //TODO: Flesh this out.
    for(size_t i = 0; i < watchlist_length; ++i){
        StockValue* previous_stock_value = user_watchlist_at(user, i);
        const StockValue* fetched_stock_value = api_get_stock_value(previous_stock_value->symbol);

        printf("fetched_stock_value current information: \n-> Symbol: %s\n-> Currency: %s\n-> Regular Market Price: %f\n-> Previous Close: %f\n", 
                fetched_stock_value->symbol, fetched_stock_value->currency, fetched_stock_value->regularMarketPrice, fetched_stock_value->previousClose);
    
        //TODO: This is only for testing purposes.
        previous_stock_value->regularMarketPrice = fetched_stock_value->regularMarketPrice;
    }

    watchlist_refresh_view(GTK_LIST_BOX(context->ui.watchlist), user);

    return G_SOURCE_CONTINUE;
}

