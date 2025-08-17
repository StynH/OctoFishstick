#ifndef OCTO_FISHSTICK_APP
#define OCTO_FISHSTICK_APP

#include "gtk/gtk.h"
#include "user.h"
#include "ui.h"

typedef struct AppCtx{
    GtkApplication* app;
    User* user;
    UICtx ui;
} AppCtx;

GtkApplication* app_create();
int app_run(AppCtx* context, int argc, char* argv[]);

#endif
