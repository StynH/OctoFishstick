#include "glib.h"
#include "src/api.h"
#include "src/background/api_worker.h"
#include "src/user.h"
#include "window.h"
#include <curl/curl.h>
#include "app.h"

static const int BACKGROUND_WORKER_INTERVAL_IN_MS = 60000;

int main(int argc, char* argv[]){
    curl_global_init(CURL_GLOBAL_ALL);

    g_print("Starting...\n");
    AppCtx* context = malloc(sizeof(AppCtx));
    context->app = app_create();
    context->user = user_create();

    //TODO: Remove these test tickers.
    const char* test_symbols[] = {
        "XAIX.DE",
        "VUAA.DE",
        "DFEN.DE",
        "ETH-EUR"
    };

    StockValue** stock_values = api_get_stock_values(test_symbols, 4);
    for(size_t i = 0; i < 4; ++i){
        user_watchlist_add_ticker(context->user, stock_values[i]);
    }

    //TODO: This can be placed somewhere cleaner?
    g_timeout_add(BACKGROUND_WORKER_INTERVAL_IN_MS, api_worker_fetch_tickers, context);

    g_print("[OCTO]: Creating window...\n");
    window_initialize(context);

    g_print("[OCTO]: Starting app...\n");
    return app_run(context, argc, argv);
}
