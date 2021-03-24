#pragma once

#include <string>

#include <sys/types.h>

/*
 * fmtMode - converts a mode_t value to its graphical representation
 *           (e.g. 'drwxr-xr-x')
 */

std::string fmtMode(mode_t);
