#include "app.h"
#include "glib.h"
#include "actions.h"
#include "gtk/gtkshortcut.h"
#include "src/resources.h"
#include <assert.h>
#include <gtk/gtk.h>
#include <livechart.h>
#include <string.h>

static const char* const APP_TITLE = "OctoFishstick";
static const int WINDOW_WIDTH = 1024;
static const int WINDOW_HEIGHT = 768;
static const int SIDEBAR_WIDTH = 128;
static const int SPACING = 12;

static void window_activate(GtkApplication* app, gpointer user_data);
static void window_initialize_layout(GtkWindow* window, AppCtx* context);

static GtkWidget* window_build_header();
static GtkWidget* window_build_sidepanel(AppCtx* context);
static GtkWidget* window_build_mainpanel();
static GtkWidget* window_build_footer();

typedef struct ActionButton{
    const char* text;
    void (*handler)(GtkWidget* widget, gpointer user_data);
} ActionButton;

void window_initialize(AppCtx* context){
    g_signal_connect(context->app, "activate", G_CALLBACK(window_activate), context);
}

static void window_activate(GtkApplication* app, gpointer user_data){
    AppCtx* context = user_data;

    resources_load_css();

    GtkWidget* window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), APP_TITLE);
    gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_WIDTH, WINDOW_HEIGHT);
    gtk_window_set_resizable(GTK_WINDOW(window), false);

    window_initialize_layout(GTK_WINDOW(window), context);
    context->ui.main_window = GTK_WINDOW(window);

    gtk_window_present(GTK_WINDOW(window));
}


static void window_initialize_layout(GtkWindow* window, AppCtx* context){
    GtkWidget* root = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_window_set_child(window, root);

    GtkWidget* header = window_build_header();
    gtk_window_set_titlebar(window, header);

    GtkWidget* paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_widget_set_hexpand(paned, true);
    gtk_widget_set_vexpand(paned, true);

    GtkWidget* main = window_build_mainpanel();
    GtkWidget* sidepanel = window_build_sidepanel(context);

    gtk_paned_set_start_child(GTK_PANED(paned), sidepanel);
    gtk_paned_set_end_child  (GTK_PANED(paned), main);

    gtk_paned_set_shrink_start_child(GTK_PANED(paned), false);
    gtk_paned_set_shrink_end_child  (GTK_PANED(paned), false);
    gtk_paned_set_resize_end_child(GTK_PANED(paned), true);
    gtk_paned_set_position(GTK_PANED(paned), SIDEBAR_WIDTH);

    GtkWidget* footer = window_build_footer();

    gtk_box_append(GTK_BOX(root), paned);
    gtk_box_append(GTK_BOX(root), footer);
}

static void window_add_ticker(GtkWidget* widget, gpointer user_data){
    action_call(widget, ACTION_ADD_TICKER, user_data);
}

static GtkWidget* window_build_header(){
    GtkWidget* header = gtk_header_bar_new();
    GtkWidget* text = gtk_label_new("OctoFishstick"); //TODO: Expand with search option for tickers
    gtk_header_bar_set_title_widget(GTK_HEADER_BAR(header), text);
    gtk_widget_add_css_class(header, "header");
    return header;
}

static GtkWidget* window_build_sidepanel(AppCtx* context){
    GtkWidget* sidepanel = gtk_box_new(GTK_ORIENTATION_VERTICAL, SPACING);
    gtk_widget_set_size_request(sidepanel, SIDEBAR_WIDTH, -1);

    ActionButton buttons[] = {
        { "Add Ticker", window_add_ticker }
    };

    for(size_t i = 0; i < sizeof(buttons)/sizeof(buttons[0]); ++i){
        ActionButton action_button = buttons[i];
        GtkWidget* button = gtk_button_new_with_label(action_button.text);
        gtk_widget_add_css_class(button, "primary");
        g_signal_connect(button, "clicked", G_CALLBACK(action_button.handler), context);

        gtk_box_append(GTK_BOX(sidepanel), button);
    }

    return sidepanel;
}

static GtkWidget* window_build_mainpanel(){ 
    GtkWidget* main = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, SPACING);
    gtk_widget_set_hexpand(main, true);
    gtk_widget_set_vexpand(main, true);

    LiveChartConfig* config = live_chart_config_new();

    LiveChartStaticStaticChart* chart = live_chart_static_static_chart_new(config);
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

    gtk_widget_set_hexpand(GTK_WIDGET(chart), true);
    gtk_widget_set_vexpand(GTK_WIDGET(chart), true);
    gtk_widget_set_halign(GTK_WIDGET(chart), GTK_ALIGN_FILL);
    gtk_widget_set_valign(GTK_WIDGET(chart), GTK_ALIGN_FILL);

    gtk_box_append(GTK_BOX(main), GTK_WIDGET(chart));
    return main;
}

static GtkWidget* window_build_footer(){
    GtkWidget* footer = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_widget_add_css_class(footer, "footer");
    gtk_widget_set_margin_top(footer, 0);

    GtkWidget* text = gtk_label_new("Hello I am footer!");
    gtk_box_append(GTK_BOX(footer), text);
    return footer;
}
