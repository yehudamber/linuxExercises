#include "makedir.h"

#include <cerrno>
#include <system_error>

#include <sys/stat.h>

void makeDir(const std::string& path)
{
    constexpr auto mode = S_IRWXU | S_IRWXG | S_IRWXO; // 0777

    if (::mkdir(path.c_str(), mode) < 0)
    {
        auto savedErrno = errno;
        throw std::system_error(savedErrno, std::generic_category(),
                                "failed to make " + path);
    }
}
