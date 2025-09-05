#ifndef OCTO_FISHSTICK_WATCHLIST
#define OCTO_FISHSTICK_WATCHLIST

#include "src/user.h"
#include <gtk/gtk.h>

int watchlist_refresh_view(gpointer data);
GtkWidget* watchlist_build_view(const User* user);

#endif
