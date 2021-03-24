#pragma once

#include <string>

#include <sys/types.h>

/*
 * fmtMode - a function that converts a mode_t value to its graphical
 *           representation (eg. 'drwxr-xr-x')
 */

std::string fmtMode(mode_t);
