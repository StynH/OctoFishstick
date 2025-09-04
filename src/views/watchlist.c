#include "watchlist.h"
#include "gtk/gtk.h"
#include "src/api.h"

static const char* const WATCHLIST_CLASS = "user-watchlist";
static const char* const WATCHLIST_CARD_CLASS = "user-watchlist-card";

static GtkWidget* watchlist_card_build_view(const StockValue* stock_value);

void watchlist_refresh_view(GtkListBox* watchlist, const User* user){
    gtk_list_box_invalidate_filter(watchlist);
    gtk_list_box_remove_all(watchlist);

    printf("Refreshing watchlist...\n");

    for (size_t i = 0; i < user_watchlist_length(user); ++i) {
        StockValue* ticker = (StockValue*)user_watchlist_at(user, i);
        GtkWidget* row = watchlist_card_build_view(ticker);
        gtk_list_box_append(watchlist, row);
    }

    gtk_widget_add_css_class(GTK_WIDGET(watchlist), WATCHLIST_CLASS);
}

GtkWidget* watchlist_build_view(const User* user){
    GtkWidget* watchlist = gtk_list_box_new();
    gtk_widget_set_vexpand(watchlist, TRUE);
    gtk_widget_set_hexpand(watchlist, TRUE);
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(watchlist), GTK_SELECTION_NONE);
    gtk_widget_set_name(watchlist, "watchlist");

    watchlist_refresh_view(GTK_LIST_BOX(watchlist), user);

    return watchlist;
}

static GtkWidget* watchlist_card_build_view(const StockValue* stock_value){
    GtkWidget* box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_widget_add_css_class(box, WATCHLIST_CARD_CLASS);

    GtkWidget* symbol = gtk_label_new(stock_value->symbol);
    gtk_widget_set_hexpand(symbol, TRUE);

    char price_str[16];
    snprintf(price_str, sizeof(price_str), "$%.2f", stock_value->regularMarketPrice);
    GtkWidget* price = gtk_label_new(price_str);

    gtk_box_append(GTK_BOX(box), symbol);
    gtk_box_append(GTK_BOX(box), price);

    //TODO: Tweak margins here for box.
    GtkWidget* frame = gtk_frame_new(NULL);
    gtk_frame_set_child(GTK_FRAME(frame), box);
    gtk_widget_set_margin_top(frame, 6);
    gtk_widget_set_margin_bottom(frame, 6);
    gtk_widget_set_margin_start(frame, 12);
    gtk_widget_set_margin_end(frame, 12);

    return frame;
}
