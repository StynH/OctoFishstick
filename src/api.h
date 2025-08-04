#ifndef OCTO_FISHSTICK_API
#define OCTO_FISHSTICK_API

typedef struct StockValue{
    char* currency;
    char* symbol;
    double regularMarketPrice;
    double previousClose;
} StockValue;

StockValue* api_get_stock_value(const char* ticker);
void api_free_stock_value(StockValue* value);

#endif
