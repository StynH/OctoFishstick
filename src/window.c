#include "api.h"
#include "glib-object.h"
#include "glib.h"
#include <gtk/gtk.h>

static const char* const APP_TITLE = "OctoFishstick";
static const int WINDOW_WIDTH = 512;
static const int WINDOW_HEIGHT = 768;

static void window_activate(GtkApplication* app, gpointer user_data);
static void window_initialize_layout(GtkWindow* window);

static void window_fetch_data_click(GtkWidget* widget, gpointer data);

void window_initialize(GtkApplication* app){
    g_signal_connect(app, "activate", G_CALLBACK(window_activate), nullptr);
}

static void window_activate(GtkApplication* app, gpointer user_data){
    GtkWidget* window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), APP_TITLE);
    gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_HEIGHT, WINDOW_WIDTH);
    
    window_initialize_layout(GTK_WINDOW(window));
    gtk_window_present(GTK_WINDOW(window));
}

static void window_initialize_layout(GtkWindow* window){
    GtkWidget* box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);

    gtk_window_set_child(window, box);

    GtkWidget* button = gtk_button_new_with_label("Fetch data");
    g_signal_connect(button, "clicked", G_CALLBACK(window_fetch_data_click), nullptr);

    gtk_box_append(GTK_BOX(box), button);
}

//TODO: Temporary
static gpointer window_api_fetch(gpointer data){
    StockValue* stock = api_get_stock_value("XAIX.DE");
    if(stock){
        printf("Stock current information: \n-> Symbol: %s\n-> Currency: %s\n-> Regular Market Price: %f\n-> Previous Close: %f\n", 
               stock->symbol, stock->currency, stock->regularMarketPrice, stock->previousClose);
        api_free_stock_value(stock);
    }
    return nullptr;
}

//TODO: Temporary
static void window_fetch_data_click(GtkWidget* widget, gpointer data){
    g_print("Fetching data.");
    g_thread_new("api-fetch-task", window_api_fetch, nullptr);
}
