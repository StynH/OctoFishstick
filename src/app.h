#ifndef OCTO_FISHSTICK_APP
#define OCTO_FISHSTICK_APP

#include "gtk/gtk.h"

GtkApplication* app_create();
int app_run(GtkApplication* app);

#endif
