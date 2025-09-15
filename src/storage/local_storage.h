#ifndef OCTO_FISHSTICK_STORAGE_LOCAL
#define OCTO_FISHSTICK_STORAGE_LOCAL

#include "src/user.h"

void local_storage_setup();
void local_storage_store_user(const User* user);
User* local_storage_load_user();

#endif
