#include "time_helpers.h"
#include <assert.h>
#include <time.h>

void time_get_current(char* buffer, size_t buffer_length, const char* format)
{
    assert(buffer);
    assert(buffer_length > 0);
    assert(format);

    time_t now = time(nullptr);

    struct tm local_time;
    localtime_r(&now, &local_time);

    strftime(buffer, buffer_length, format, &local_time);
}
