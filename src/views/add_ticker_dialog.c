#include "add_ticker_dialog.h"
#include "glib-object.h"
#include "src/api.h"
#include <glib.h>
#include <gtk/gtk.h>
#include <stdlib.h>

static const char* const WINDOW_TITLE = "Add Ticker";
static const int POPUP_WIDTH = 512;
static const int POPUP_HEIGHT = 256;
static const int POPUP_SPACING = 12;

static void add_ticker_dialog_close(GtkWidget* caller, gpointer user_data);

static void add_ticker_dialog_add_ticker(GtkWidget* caller, gpointer user_data);
static gpointer add_ticker_dialog_add_ticker_thread(gpointer user_data);

typedef struct AddTickerCtx{
    AppCtx* app;
    GtkEntry* entry;
} AddTickerCtx;

static void add_ticker_ctx_free(AddTickerCtx* context){
    free(context);
}

typedef struct FetchTickerCtx{
    AppCtx* app;
    char* ticker;
} FetchTickerCtx;

static void fetch_ticker_ctx_free(FetchTickerCtx* context){
    free(context->ticker);
    free(context);
}

GtkWindow* add_ticker_dialog_create(AppCtx* context){
    GtkWidget* popup = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(popup), WINDOW_TITLE);
    gtk_window_set_default_size(GTK_WINDOW(popup), POPUP_WIDTH, POPUP_HEIGHT);
    gtk_window_set_transient_for(GTK_WINDOW(popup), context->ui.main_window);
    gtk_window_set_destroy_with_parent(GTK_WINDOW(popup), true);
    gtk_window_set_modal(GTK_WINDOW(popup), true);

    GtkWidget* main = gtk_box_new(GTK_ORIENTATION_VERTICAL, POPUP_SPACING);

    GtkWidget* input = gtk_entry_new();
    gtk_box_append(GTK_BOX(main), input);

    GtkWidget* button_container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_append(GTK_BOX(main), button_container);

    GtkWidget* close_button = gtk_button_new_with_label("Cancel");
    g_signal_connect(close_button, "clicked", G_CALLBACK(add_ticker_dialog_close), popup);
    gtk_box_append(GTK_BOX(button_container), close_button);

    AddTickerCtx* add_context = malloc(sizeof(AddTickerCtx));
    add_context->app = context;
    add_context->entry = GTK_ENTRY(input);

    GtkWidget* add_button = gtk_button_new_with_label("Add");
    g_signal_connect_data(
        add_button, "clicked", G_CALLBACK(add_ticker_dialog_add_ticker), add_context,
        (GClosureNotify)add_ticker_ctx_free, 0);
    gtk_box_append(GTK_BOX(button_container), add_button);

    gtk_window_set_child(GTK_WINDOW(popup), main);
    
    return GTK_WINDOW(popup);
}

static void add_ticker_dialog_close(GtkWidget* caller, gpointer user_data){
    GtkWindow* popup = GTK_WINDOW(user_data);
    gtk_window_close(popup);
}

static void add_ticker_dialog_add_ticker(GtkWidget* caller, gpointer user_data){
    g_print("Fetching data.");
    AddTickerCtx* context = user_data;
    char* ticker = strdup(gtk_editable_get_text(GTK_EDITABLE(context->entry)));

    if(!ticker || !*ticker){
        return;
    }

    FetchTickerCtx* fetch_context = malloc(sizeof(FetchTickerCtx));
    fetch_context->app = context->app;
    fetch_context->ticker = ticker;
    g_thread_new("api-fetch-task", add_ticker_dialog_add_ticker_thread, fetch_context);
}

static gpointer add_ticker_dialog_add_ticker_thread(gpointer user_data){
    FetchTickerCtx* context = user_data;
    api_get_stock_value(context->ticker);
    fetch_ticker_ctx_free(context);
    return G_SOURCE_REMOVE;
}
