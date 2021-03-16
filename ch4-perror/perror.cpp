#include "perror.h"

#include <cerrno>
#include <cstdio>
#include <cstring>

inline namespace MyUtils
{

void perror(const char* str)
{
    std::printf("%s: %s\n", str, std::strerror(errno));
}

} // inline namespace MyUtils
