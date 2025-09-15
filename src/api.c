#include "api.h"
#include "json_object.h"
#include "json_types.h"
#include "helpers/string_helpers.h"
#include <assert.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <json-c/json.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char* const API_URL = "https://query1.finance.yahoo.com/v8/finance/spark?symbols=%s&range=2h&interval=1m";

typedef struct MemoryRegister {
    char* data;
    size_t size;
} MemoryRegister;

static json_object* api_perform_call(const char* url);
static StockValue* api_parse_json_as_stock(const json_object* response);

StockValue* api_get_stock_value(const char* symbol){
    size_t size = snprintf(nullptr, 0, API_URL, symbol);
    char* url = malloc(size + 1);

    if(!url){
        return nullptr;
    }

    snprintf(url, size + 1, API_URL, symbol);
    
    printf("[OCTO]: Performing API call for URL: %s\n", url);
    json_object* result = api_perform_call(url);
    if(result){
        json_object* meta;
        if(json_object_object_get_ex(result, symbol, &meta)){
            StockValue* stock = api_parse_json_as_stock(meta);
            printf("[OCTO]: Finished fetching for symbol '%s'\n.", symbol);
            return stock;
        }

        printf("[OCTO]: Failed to retrieve metadata for symbol '%s'\n.", symbol);
        return nullptr;
    }

    printf("[OCTO]: Failed to perform API call.\n");
    return nullptr;
}

StockValue** api_get_stock_values(const char** symbols, size_t amount_of_symbols){
    StockValue** stocks = malloc(amount_of_symbols * sizeof(StockValue*));
    const char* url_symbols = string_join(symbols, amount_of_symbols, ",");

    size_t size = snprintf(nullptr, 0, API_URL, url_symbols);
    char* url = malloc(size + 1);

    if(!url){
        return nullptr;
    }

    snprintf(url, size + 1, API_URL, url_symbols);
    
    printf("[OCTO]: Performing API call for URL: %s\n", url);
    json_object* result = api_perform_call(url);
    if(result){
        for(size_t i = 0; i < amount_of_symbols; ++i){
            const char* symbol = symbols[i];

            json_object* meta;
            if(json_object_object_get_ex(result, symbol, &meta)){
                StockValue* stock = api_parse_json_as_stock(meta);
                stocks[i] = stock;
            }
            else{
                printf("[OCTO]: Failed to retrieve metadata for symbol '%s'\n.", symbol);
            }
        }
    }

    return stocks;
}

void api_free_stock_value(StockValue* value){
    if(value){
        free(value->symbol);
        free(value);
    }
}

static size_t api_write_memory_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    MemoryRegister* reg = (MemoryRegister*)userp;

    reg->data = realloc(reg->data, reg->size + realsize + 1);
    if(reg->data == nullptr) {
        return 0;
    }

    memcpy(&(reg->data[reg->size]), contents, realsize);
    reg->size += realsize;
    reg->data[reg->size] = 0;

    return realsize;
}

static json_object* api_perform_call(const char* url)
{
    MemoryRegister reg = {
        .data = nullptr,
        .size = 0
    };

    CURL* handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_URL, url);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, api_write_memory_callback);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&reg);
    curl_easy_setopt(handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    CURLcode result = curl_easy_perform(handle);
    json_object* json_obj = nullptr;
    if(result != CURLE_OK){
        fprintf(stderr, "Stock API call failed for URL: %s. Error: %s\n", url, curl_easy_strerror(result));
    }
    else{
        json_obj = json_tokener_parse(reg.data);
    }

    curl_easy_cleanup(handle);
    free(reg.data);

    return json_obj;
}

static StockValue* api_parse_json_as_stock(const json_object* response){
    assert(response);

    StockValue* stock_value = malloc(sizeof(StockValue));
    stock_value->symbol = strdup(json_object_get_string(json_object_object_get(response, "symbol")));

    json_object* close_arr_json = json_object_object_get(response, "close");
    if(close_arr_json != nullptr){
        size_t close_arr_length = json_object_array_length(close_arr_json);
        stock_value->currentPrice = json_object_get_double(json_object_array_get_idx(close_arr_json, close_arr_length - 1));
    }

    return stock_value;
}
