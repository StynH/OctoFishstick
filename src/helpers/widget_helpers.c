#include "widget_helpers.h"
#include <string.h>
#include <assert.h>

static bool ui_widget_matches_name(GtkWidget* widget, const char* name)
{
    const char* widget_name = gtk_widget_get_name(widget);

    if (widget_name == nullptr || name == nullptr) {
        return false;
    }

    return strcmp(widget_name, name) == 0;
}

static GtkWidget* ui_find_first_depth(GtkWidget* widget, const char* name)
{
    if (ui_widget_matches_name(widget, name)) {
        return widget;
    }

    for (GtkWidget* child = gtk_widget_get_first_child(widget); child != nullptr; child = gtk_widget_get_next_sibling(child)) {
        GtkWidget* result = ui_find_first_depth(child, name);

        if (result != nullptr) {
            return result;
        }
    }

    return nullptr;
}

static GtkWidget* ui_find_nth_depth(GtkWidget* widget, const char* name, size_t* seen, size_t target_index)
{
    if (ui_widget_matches_name(widget, name)) {
        if (*seen == target_index) {
            return widget;
        }

        (*seen)++;
    }

    for (GtkWidget* child = gtk_widget_get_first_child(widget); child != nullptr; child = gtk_widget_get_next_sibling(child)) {
        GtkWidget* result = ui_find_nth_depth(child, name, seen, target_index);

        if (result != nullptr) {
            return result;
        }
    }

    return nullptr;
}

static void ui_find_collect_depth(GtkWidget* widget, const char* name, GPtrArray* collection)
{
    if (ui_widget_matches_name(widget, name)) {
        g_ptr_array_add(collection, widget);
    }

    for (GtkWidget* child = gtk_widget_get_first_child(widget); child != nullptr; child = gtk_widget_get_next_sibling(child)) {
        ui_find_collect_depth(child, name, collection);
    }
}

GtkWidget* ui_find(GtkWidget* root, const char* name)
{
    assert(root);
    assert(name);

    return ui_find_first_depth(root, name);
}

GtkWidget* ui_find_nth(GtkWidget* root, const char* name, size_t index)
{
    assert(root);
    assert(name);

    size_t seen = 0;

    return ui_find_nth_depth(root, name, &seen, index);
}

GPtrArray* ui_find_all(GtkWidget* root, const char* name)
{
    assert(root);
    assert(name);

    GPtrArray* collection = g_ptr_array_new();

    ui_find_collect_depth(root, name, collection);

    return collection;
}
