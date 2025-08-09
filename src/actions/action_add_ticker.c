#include "action_add_ticker.h"
#include "../api.h"
#include "glib-object.h"
#include "glib.h"
#include <gtk/gtk.h>

static const char* const WINDOW_TITLE = "Add Ticker";
static const int POPUP_WIDTH = 512;
static const int POPUP_HEIGHT = 256;
static const int POPUP_SPACING = 12;

gpointer action_add_ticker_api_fetch(gpointer user_data);
void action_add_ticker_close(GtkWidget* button, gpointer user_data);
void action_add_ticker_fetch_data_click(GtkWidget* widget, gpointer data);

    void action_add_ticker_perform(){
    GtkWidget* popup = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(popup), WINDOW_TITLE);
    gtk_window_set_default_size(GTK_WINDOW(popup), POPUP_WIDTH, POPUP_HEIGHT);
    gtk_window_set_modal(GTK_WINDOW(popup), true);

    GtkWidget* main = gtk_box_new(GTK_ORIENTATION_VERTICAL, POPUP_SPACING);

    GtkWidget* input = gtk_entry_new();
    gtk_box_append(GTK_BOX(main), input);
    
    GtkWidget* button_container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_append(GTK_BOX(main), button_container);

    GtkWidget* close_button = gtk_button_new_with_label("Cancel");
    g_signal_connect(close_button, "clicked", G_CALLBACK(action_add_ticker_close), popup);
    gtk_box_append(GTK_BOX(button_container), close_button);

    GtkWidget* add_button = gtk_button_new_with_label("Add");
    g_signal_connect(add_button, "clicked", G_CALLBACK(action_add_ticker_fetch_data_click), input);
    gtk_box_append(GTK_BOX(button_container), add_button);

    gtk_window_set_child(GTK_WINDOW(popup), main);
    gtk_window_present(GTK_WINDOW(popup));
}

void action_add_ticker_close(GtkWidget* button, gpointer user_data){
    GtkWindow* popup = GTK_WINDOW(user_data);
    gtk_window_close(popup);
}

gpointer action_add_ticker_api_fetch(gpointer user_data){
    const char* ticker = (const char*)user_data;
    StockValue* stock = api_get_stock_value(ticker);
    if(stock){
        printf("Stock current information: \n-> Symbol: %s\n-> Currency: %s\n-> Regular Market Price: %f\n-> Previous Close: %f\n", 
               stock->symbol, stock->currency, stock->regularMarketPrice, stock->previousClose);
        api_free_stock_value(stock);
    }
    return nullptr;
}

void action_add_ticker_fetch_data_click(GtkWidget* widget, gpointer data){
    g_print("Fetching data.");
    const char* ticker = gtk_editable_get_text(GTK_EDITABLE(data));
    g_thread_new("api-fetch-task", action_add_ticker_api_fetch, ticker);
}
