#include "api.h"
#include "glib-object.h"
#include "glib.h"
#include <assert.h>
#include <gtk/gtk.h>
#include <livechart.h>

static const char* const APP_TITLE = "OctoFishstick";
static const int WINDOW_WIDTH = 768;
static const int WINDOW_HEIGHT = 512;

static void window_activate(GtkApplication* app, gpointer user_data);
static void window_initialize_layout(GtkWindow* window);

static void window_fetch_data_click(GtkWidget* widget, gpointer data);

void window_initialize(GtkApplication* app){
    g_signal_connect(app, "activate", G_CALLBACK(window_activate), nullptr);
}

static void window_activate(GtkApplication* app, gpointer user_data){
    GtkWidget* window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), APP_TITLE);
    gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_WIDTH, WINDOW_HEIGHT);
    
    window_initialize_layout(GTK_WINDOW(window));
    gtk_window_present(GTK_WINDOW(window));
}

static void window_initialize_layout(GtkWindow* window){
    GtkWidget* box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);

    gtk_window_set_child(window, box);

    //GtkWidget* button = gtk_button_new_with_label("Fetch data");
    //g_signal_connect(button, "clicked", G_CALLBACK(window_fetch_data_click), nullptr);

    //gtk_box_append(GTK_BOX(box), button);

    //TODO: Testing the LiveChart libraryi
    LiveChartConfig* config = live_chart_config_new();

    LiveChartStaticStaticChart* chart = live_chart_static_static_chart_new(config);

    gtk_widget_set_vexpand(GTK_WIDGET(chart), true);
    gtk_widget_set_hexpand(GTK_WIDGET(chart), true);

    LiveChartStaticStaticValues* values = live_chart_static_static_values_new();
    LiveChartStaticStaticLine* renderer = live_chart_static_static_line_new(values);
    LiveChartStaticStaticSerie* heap = live_chart_static_static_serie_new("HEAP", 
                                                                          LIVE_CHART_STATIC_STATIC_SERIE_RENDERER(renderer));  
    live_chart_static_static_chart_add_serie(chart, heap);

    GeeArrayList* categories = gee_array_list_new(G_TYPE_STRING, (GBoxedCopyFunc)g_strdup, (GDestroyNotify)g_free, nullptr, nullptr, nullptr);
    gee_abstract_collection_add(GEE_ABSTRACT_COLLECTION(categories), "paris");
    gee_abstract_collection_add(GEE_ABSTRACT_COLLECTION(categories), "london");
    gee_abstract_collection_add(GEE_ABSTRACT_COLLECTION(categories), "mexico");
    gee_abstract_collection_add(GEE_ABSTRACT_COLLECTION(categories), "seville");

    live_chart_static_static_chart_set_categories(chart, categories);
    live_chart_static_static_serie_add(heap, "paris", 5000);
    live_chart_static_static_serie_add(heap, "london", 7000);
    live_chart_static_static_serie_add(heap, "mexico", 0);
    live_chart_static_static_serie_add(heap, "seville", 3000);

    gtk_widget_set_size_request(GTK_WIDGET(chart), WINDOW_WIDTH, WINDOW_HEIGHT);
    live_chart_config_set_width(config, WINDOW_WIDTH);
    live_chart_config_set_height(config, WINDOW_HEIGHT);
    gtk_box_append(GTK_BOX(box), GTK_WIDGET(chart));
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
