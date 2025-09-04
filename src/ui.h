#ifndef OCTO_FISHSTICK_UI
#define OCTO_FISHSTICK_UI

#include <gtk/gtk.h>

typedef struct UICtx{
    GtkWindow* main_window;
    GtkWidget* header;
    GtkWidget* footer;
} UICtx;

#endif
