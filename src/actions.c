#include "actions.h"
#include "app.h"
#include "glib.h"
#include "views/add_ticker_dialog.h"
#include <gtk/gtk.h>

typedef struct ActionFn{
    ACTION id;
    void (*callback)(GtkWidget* caller, gpointer user_data);
} ActionFn;

static void action_add_ticker(GtkWidget* caller, gpointer user_data);

static ActionFn action_table[] = {
    { ACTION_ADD_TICKER, action_add_ticker }
};

void action_call(GtkWidget* caller, ACTION action, gpointer user_data){
    const size_t action_table_size = sizeof(action_table) / sizeof(action_table[0]);
    for(size_t i = 0; i < action_table_size; ++i){
        if(action_table[i].id == action){
            action_table[i].callback(caller, user_data);
            return;
        }
    }
}

static void action_add_ticker(GtkWidget* caller [[__maybe_unused__]], gpointer user_data){
    AppCtx* context = user_data;

    GtkWindow* add_ticker_dialog = GTK_WINDOW(add_ticker_dialog_create(context));
    gtk_window_present(add_ticker_dialog);
}

