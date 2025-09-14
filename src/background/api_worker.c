#include "api_worker.h"
#include "glib.h"
#include "src/api.h"
#include "src/app.h"
#include "src/views/footer.h"
#include "src/views/watchlist.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>

static char** api_worker_build_symbol_list(const User* user);

gboolean api_worker_fetch_tickers(gpointer data){
    AppCtx* context = (AppCtx*)data;
    User* user = context->user;
    size_t amount_of_symbols = user_watchlist_length(user);

    char** symbols = api_worker_build_symbol_list(user);
    StockValue** fetched_data = api_get_stock_values((const char**)symbols, amount_of_symbols);

    //TODO: Counting the amount of current stocks is bad
    //because it isn't guaranteed the same amount comes back.
    //Rework response type.
    for(size_t i = 0; i < amount_of_symbols; ++i){
        StockValue* fetched_value = fetched_data[i];
        StockValue* old_value = user_watchlist_by_symbol(user, fetched_value->symbol);
        if(old_value == nullptr){
            printf("[OCTO] Fetched data for symbol '%s' but it was not found in the users watchlist.\n", fetched_value->symbol);
            continue;
        }

        //TODO: Feels off to do this.
        old_value->previousClose = fetched_value->previousClose;
    }

    g_main_context_invoke(
        nullptr, 
        watchlist_refresh_view,
        context
    );

    g_main_context_invoke(
        nullptr, 
        footer_update, 
        context
    );

    for(size_t i = 0; i < amount_of_symbols; ++i){
        api_free_stock_value(fetched_data[i]);
        free(symbols[i]);
    }

    free(symbols);
    free(fetched_data);
    return G_SOURCE_CONTINUE;
}

static char** api_worker_build_symbol_list(const User* user){
    assert(user);

    size_t amount_of_symbols = user_watchlist_length(user);
    char** symbols = malloc(amount_of_symbols * sizeof(char*));

    for(size_t i = 0; i < amount_of_symbols; ++i){
        symbols[i] = strdup(user_watchlist_at(user, i)->symbol);
    }

    return symbols;
}
