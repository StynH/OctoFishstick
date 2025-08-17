#ifndef OCTO_FISHSTICK_WATCHLIST
#define OCTO_FISHSTICK_WATCHLIST

#include "src/user.h"
#include <gtk/gtk.h>

void watchlist_refresh_view(GtkListBox* watchlist, const User* user);
GtkWidget* watchlist_build_view(const User* user);

#endif
