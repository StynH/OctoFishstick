#ifndef OCTO_FISHSTICK_ACTIONS
#define OCTO_FISHSTICK_ACTIONS

#include "glib.h"
#include <gtk/gtk.h>

typedef enum ACTION {
    ACTION_ADD_TICKER
} ACTION;

void action_call(GtkWidget* caller, ACTION action, gpointer user_data);

#endif
