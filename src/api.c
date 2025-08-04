#include "api.h"
#include "json_object.h"
#include <curl/curl.h>
#include <curl/easy.h>
#include <json-c/json.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char* const API_URL = "https://query1.finance.yahoo.com/v8/finance/chart/%s?range=1d&interval=1m";

typedef struct MemoryRegister {
    char* data;
    size_t size;
} MemoryRegister;

json_object* api_perform_call(const char* url);
StockValue* api_parse_json_as_stock(const json_object* response);

StockValue* api_get_stock_value(const char* ticker){
    size_t size = snprintf(nullptr, 0, API_URL, ticker);
    char* url = malloc(size + 1);

    if(!url){
        return nullptr;
    }

    snprintf(url, size + 1, API_URL, ticker);
    
    printf("Performing API call for URL: %s\n", url);
    json_object* result = api_perform_call(url);
    if(result){
        StockValue* stock = api_parse_json_as_stock(result);

        return stock;
    }

    return nullptr;
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

json_object* api_perform_call(const char* url)
{
    CURL* handle;
    CURLcode result;

    MemoryRegister reg = {
        .data = nullptr,
        .size = 0
    };

    handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_URL, url);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, api_write_memory_callback);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&reg);
    curl_easy_setopt(handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    result = curl_easy_perform(handle);
    json_object* jsonObject = nullptr;
    if(result != CURLE_OK){
        fprintf(stderr, "Stock API call failed for URL: %s. Error: %s\n", url, curl_easy_strerror(result));
    }
    else{
        jsonObject = json_tokener_parse(reg.data);
        //printf("JSON from Stock API call:\n %s \n", json_object_to_json_string_ext(jsonObject, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY));
    }

    curl_easy_cleanup(handle);
    free(reg.data);

    return jsonObject;
}

void api_free_stock_value(StockValue* value){
    if(value){
        free(value->currency);
        free(value->symbol);
        free(value);
    }
}

StockValue* api_parse_json_as_stock(const json_object* response){
    if(!response){
        return nullptr;
    }

    json_object* chart;
    if(json_object_object_get_ex(response, "chart", &chart)){
        json_object* result;
        if(json_object_object_get_ex(chart, "result", &result)){
            json_object* root = json_object_array_get_idx(result, 0);
            json_object* meta;
            if(json_object_object_get_ex(root, "meta", &meta)){
                StockValue* stockValue = malloc(sizeof(StockValue));
                stockValue->currency = strdup(json_object_get_string(json_object_object_get(meta, "currency")));
                stockValue->symbol = strdup(json_object_get_string(json_object_object_get(meta, "symbol")));
                stockValue->regularMarketPrice = json_object_get_double(json_object_object_get(meta, "regularMarketPrice"));
                stockValue->previousClose = json_object_get_double(json_object_object_get(meta, "previousClose"));

                return stockValue;
            }
        }
    }

    fprintf(stderr, "Unable to parse JSON from Stock API.\n");
    return nullptr;
}
