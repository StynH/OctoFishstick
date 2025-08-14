#ifndef OCTO_FISHSTICK_USER
#define OCTO_FISHSTICK_USER

#include <stddef.h>
typedef struct User User;

User* user_create();
void user_free(User* user);

void user_watchlist_add_ticker(User* user, const char* ticker);
size_t user_watchlist_length(const User* user);
const char* user_watchlist_at(const User* user, size_t index);

#endif
