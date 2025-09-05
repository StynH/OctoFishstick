#include "footer.h"
#include "glib.h"
#include "gtk/gtk.h"
#include "src/app.h"
#include "src/helpers/time_helpers.h"
#include "src/helpers/widget_helpers.h"

int footer_update(gpointer data){
    AppCtx* context = (AppCtx*)data;
    GtkWidget* footer = context->ui.footer;

    GtkWidget* last_update_label = ui_find(footer, "last_update_label");
    if(last_update_label == nullptr){
        printf("[OCTO]: Unable to find last update label in footer.");
        return G_SOURCE_REMOVE;
    }

    char time_buffer[64];
    time_get_current(time_buffer, sizeof(time_buffer), "Last Update: %H:%M");
    gtk_label_set_text(GTK_LABEL(last_update_label), time_buffer);

    return G_SOURCE_REMOVE;
}
