#include "glib-object.h"
#include "glib.h"
#include <gtk/gtk.h>

typedef struct ActionButton{
    const char* text;
    void (*handler)(GtkWidget* widget, gpointer* user_data);
} ActionButton;

void actions_add_ticker(GtkWidget* widget, gpointer* user_data){
    printf("TODO!\n");
}

void actions_sidepanel_initialize(GtkWidget* container){
    ActionButton buttons[] = {
        { "Add Ticker", actions_add_ticker }
    };

    for(size_t i = 0; i < sizeof(buttons)/sizeof(buttons[0]); ++i){
        ActionButton action_button = buttons[i];
        GtkWidget* button = gtk_button_new_with_label(action_button.text);
        g_signal_connect(button, "clicked", G_CALLBACK(action_button.handler), nullptr);

        gtk_box_append(GTK_BOX(container), button);
    }
}
