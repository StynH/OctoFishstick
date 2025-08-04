#include "api.h"
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

void api_perform_call(const char* null);

StockValue* api_get_stock_value(const char* ticker){
    size_t size = snprintf(nullptr, 0, API_URL, ticker);
    char* url = malloc(size + 1);

    if(!url){
        return nullptr;
    }

    snprintf(url, size + 1, API_URL, ticker);
    
    printf("Performing API call for URL: %s\n", url);
    api_perform_call(url);

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

void api_perform_call(const char* url)
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
    if(result != CURLE_OK){
        fprintf(stderr, "Stock API call failed for URL: %s. Error: %s\n", url, curl_easy_strerror(result));
    }
    else{
        json_object* jsonObject = json_tokener_parse(reg.data);
        printf("JSON from Stock API call:\n %s \n", json_object_to_json_string_ext(jsonObject, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY));
    }

    curl_easy_cleanup(handle);
    free(reg.data);
}
