#include "glib.h"
#include "src/api.h"
#include <assert.h>

typedef struct User{
    GPtrArray* watchlist;
} User;

void user_free_stock_value(gpointer data);

User* user_create(){
    User* user = calloc(1, sizeof(*user));
    user->watchlist = g_ptr_array_new_with_free_func(user_free_stock_value);

    return user;
}

void user_free(User* user){
    assert(user);
    g_ptr_array_free(user->watchlist, true);
    free(user);
}

static gboolean user_watchlist_compare(gconstpointer left, gconstpointer right){
    StockValue* left_value = (StockValue*)left;
    StockValue* right_value = (StockValue*)right;
    return strcmp((const char*)left_value->symbol, (const char*)right_value->symbol) == 0;
}

void user_watchlist_add_ticker(User* user, StockValue* stock_value){
    assert(user);
    assert(stock_value);

    if(g_ptr_array_find_with_equal_func(user->watchlist, stock_value, user_watchlist_compare, nullptr)){
        return;
    }

    g_ptr_array_add(user->watchlist, stock_value);
}

size_t user_watchlist_length(const User* user){
    assert(user);

    return user->watchlist->len;
}

StockValue* user_watchlist_at(const User* user, size_t index){
    assert(user);
    assert(index < user->watchlist->len);

    return (StockValue*)g_ptr_array_index(user->watchlist, index);
}

void user_free_stock_value(gpointer data){
    StockValue* stock_value = (StockValue*)data;
    api_free_stock_value(stock_value);
}
