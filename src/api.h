#ifndef OCTO_FISHSTICK_API
#define OCTO_FISHSTICK_API

#include <stddef.h>

typedef struct StockValue{
    char* symbol;
    double currentPrice;
} StockValue;

StockValue* api_get_stock_value(const char* symbol);
StockValue** api_get_stock_values(const char** symbols, size_t amount_of_symbols);
void api_free_stock_value(StockValue* value);

#endif
