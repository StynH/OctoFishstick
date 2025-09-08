#include <stdlib.h>
#include <string.h>

char* string_join(const char** items, size_t count, const char* delimiter){
    if(items == nullptr || count == 0 || delimiter == nullptr){
        return nullptr;
    }

    size_t delimiter_len = strlen(delimiter);
    size_t total_length = 0;

    for(size_t i = 0; i < count; ++i){
        if(items[i]){
            total_length += strlen(items[i]);
        }
        if(i + 1 < count){
            total_length += delimiter_len;
        }
    }

    char* result = malloc(total_length + 1);
    if(result == nullptr){
        return nullptr;
    }

    size_t offset = 0;
    for(size_t i = 0; i < count; ++i){
        if(items[i]){
            size_t len = strlen(items[i]);
            memcpy(result + offset, items[i], len);
            offset += len;
        }
        if(i + 1 < count){
            memcpy(result + offset, delimiter, delimiter_len);
            offset += delimiter_len;
        }
    }

    result[offset] = '\0';
    return result;
}
