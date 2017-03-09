#ifndef __UTILS_H
#define __UTILS_H
#include "common.h"

void printError(cl_int error);

void check_error(cl_int x, std::string message);

// Returns the platform name.
std::string getPlatformName(cl_platform_id pid);
// Searches all platforms for the first platform whose name
// contains the search string (case-insensitive).
cl_platform_id findPlatform(const char *platform_name_search);

#endif
