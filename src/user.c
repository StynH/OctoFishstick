#include "user.h"
#include "glib.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct User{
    GPtrArray* watchlist;
} User;

User* user_create(){
    User* user = calloc(1, sizeof(*user));
    user->watchlist = g_ptr_array_new_with_free_func(free);

    return user;
}

void user_free(User* user){
    assert(user);
    g_ptr_array_free(user->watchlist, true);
    free(user);
}

static gboolean user_watchlist_compare(gconstpointer left, gconstpointer right){
    return strcmp((const char*)left, (const char*)right) == 0;
}

void user_watchlist_add_ticker(User* user, const char* ticker){
    assert(user);
    assert(ticker);

    if(g_ptr_array_find_with_equal_func(user->watchlist, ticker, user_watchlist_compare, nullptr)){
        return;
    }

    g_ptr_array_add(user->watchlist, strdup(ticker));
}

size_t user_watchlist_length(const User* user){
    assert(user);

    return user->watchlist->len;
}

const char* user_watchlist_at(const User* user, size_t index){
    assert(user);
    assert(index < user->watchlist->len);

    return (const char*)g_ptr_array_index(user->watchlist, index);
}
