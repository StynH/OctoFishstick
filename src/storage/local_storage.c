#include "local_storage.h"
#include "json_object.h"
#include "json_types.h"
#include "src/api.h"
#include "src/helpers/macro.h"
#include <linux/limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#define XDG_DATA_HOME "XDG_DATA_HOME"
#define DEFAULT_APP_DIR "octofishstick"
#define DEFAULT_HOME ".local/share"
#define DATA_FILE "octofishstick_data.json"

static char* local_storage_get_path();
static json_object* local_storage_watchlist_to_json(const User* user);
static void local_storage_write_to_file(const char* json_str);

void local_storage_setup(){
    char* path = local_storage_get_path();

    struct stat st = {0};
    if(stat(path, &st) == -1){
        printf("[OCTO]: Creating user dir at '%s'.\n", path);
        if(mkdir(path, 0700) == -1){
            panic("[OCTO]: Failed to create user dir.");
        }
    }

    free(path);
}

static char* local_storage_get_path(){
    char* path = malloc(PATH_MAX);

    const char* xdg_data_home = getenv(XDG_DATA_HOME);
    if(xdg_data_home != nullptr){
        snprintf(path, PATH_MAX, "%s/%s", xdg_data_home, DEFAULT_APP_DIR);
        return path;
    }

    const char* home = getenv("HOME");
    snprintf(path, PATH_MAX, "%s/%s/%s", home, DEFAULT_HOME, DEFAULT_APP_DIR);
    return path;
}

void local_storage_store_user(const User* user){
    json_object* user_json = json_object_new_object();
    json_object_object_add(user_json, "watchlist", local_storage_watchlist_to_json(user));

    const char* json_str = json_object_to_json_string_ext(user_json, JSON_C_TO_STRING_PRETTY);
    local_storage_write_to_file(json_str);

    json_object_put(user_json);
}

static json_object* local_storage_watchlist_to_json(const User* user){
    json_object* watchlist_json = json_object_new_array();

    size_t amount_of_symbols = user_watchlist_length(user);
    for(size_t i = 0; i < amount_of_symbols; ++i){
        StockValue* value = user_watchlist_at(user, i);
        json_object* value_json = json_object_new_object();

        json_object_object_add(value_json, "symbol", json_object_new_string(value->symbol));
        
        json_object_array_add(watchlist_json, value_json);
    }

    return watchlist_json;
}

static void local_storage_write_to_file(const char* json_str){
    char* relative_path = local_storage_get_path();
    char file_path[PATH_MAX];

    snprintf(file_path, PATH_MAX, "%s/%s", relative_path, DATA_FILE);

    FILE* file_handle = fopen(file_path, "w");
    if(file_handle == nullptr){
       panic("Unable to create/open data file '%s'.", file_path); 
    }

    fputs(json_str, file_handle);
    fclose(file_handle); 

    free(relative_path);
}

User* local_storage_load_user(){
    //TODO: Implement
    return nullptr;
}
