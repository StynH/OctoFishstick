#include "watchlist.h"
#include "glib.h"
#include "glibconfig.h"
#include "gtk/gtk.h"
#include "gtk/gtkshortcut.h"
#include "src/api.h"
#include "src/app.h"
#include "src/helpers/widget_helpers.h"
#include <stdio.h>

static const char* const WATCHLIST_CLASS = "user-watchlist";
static const char* const WATCHLIST_CARD_CLASS = "user-watchlist-card";

static GtkWidget* watchlist_card_build_view(const StockValue* stock_value);

int watchlist_refresh_view(gpointer data){
    AppCtx* context = (AppCtx*)data;
    GtkListBox* watchlist = GTK_LIST_BOX(context->ui.watchlist);
    User* user = context->user;

    gtk_list_box_invalidate_filter(watchlist);

    printf("Refreshing watchlist...\n");

    for (size_t i = 0; i < user_watchlist_length(user); ++i) {
        StockValue* ticker = (StockValue*)user_watchlist_at(user, i);
        
        //Frame -> Box -> Labels
        for (GtkWidget* child = gtk_widget_get_first_child (GTK_WIDGET(watchlist));
            child != nullptr;
            child = gtk_widget_get_next_sibling(child))
        {
            GtkWidget* box = gtk_widget_get_first_child(child);
            GtkWidget* symbol_label = ui_find(box, "symbol_label");
            if(symbol_label == nullptr){
                printf("[OCTO]: Unable to find symbol label in box.");
                continue;
            }

            const char* value = gtk_label_get_text(GTK_LABEL(symbol_label));
            if(strcmp(value, ticker->symbol) == 0){
                GtkWidget* value_label = ui_find(box, "value_label");
                if(value_label == nullptr){
                    printf("[OCTO]: Unable to find value label for '%s'.", ticker->symbol);
                    break;
                }

                char price_str[16];
                snprintf(price_str, sizeof(price_str), "€%.2f", ticker->currentPrice);
                gtk_label_set_text(GTK_LABEL(value_label), price_str);
                break;
            }
        }
    }

    gtk_widget_add_css_class(GTK_WIDGET(watchlist), WATCHLIST_CLASS);
    return G_SOURCE_REMOVE;
}

GtkWidget* watchlist_build_view(const User* user){
    GtkWidget* watchlist = gtk_list_box_new();
    gtk_widget_set_vexpand(watchlist, TRUE);
    gtk_widget_set_hexpand(watchlist, TRUE);
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(watchlist), GTK_SELECTION_NONE);
    gtk_widget_set_name(watchlist, "watchlist");

    for (size_t i = 0; i < user_watchlist_length(user); ++i) {
        StockValue* ticker = (StockValue*)user_watchlist_at(user, i);
        GtkWidget* row = watchlist_card_build_view(ticker);
        gtk_list_box_append(GTK_LIST_BOX(watchlist), row);
    }

    gtk_widget_add_css_class(GTK_WIDGET(watchlist), WATCHLIST_CLASS);

    return watchlist;
}

static GtkWidget* watchlist_card_build_view(const StockValue* stock_value){
    GtkWidget* box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_widget_add_css_class(box, WATCHLIST_CARD_CLASS);

    GtkWidget* symbol = gtk_label_new(stock_value->symbol);
    gtk_widget_set_hexpand(symbol, TRUE);
    gtk_widget_set_name(symbol, "symbol_label");

    char price_str[16];
    snprintf(price_str, sizeof(price_str), "€%.2f", stock_value->currentPrice);
    GtkWidget* price = gtk_label_new(price_str);
    gtk_widget_set_name(price, "value_label");

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
