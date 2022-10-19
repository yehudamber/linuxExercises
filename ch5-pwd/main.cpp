/****************************************************
 * ch5-pwd - Prints the working directory full path *
 ****************************************************/

#include <cerrno>
#include <iostream>
#include <stdexcept>
#include <string>
#include <system_error>
#include <vector>

#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int /*argc*/, char* argv[]) try
{
    auto path = std::vector<std::string>();
    while (true) // iterate from current WD to root directory
    {
        struct stat dotStat;
        if (::stat(".", &dotStat) < 0)
        {
            auto savedErrno = errno;
            throw std::system_error(savedErrno, std::generic_category(),
                                    "cannot stat a directory in the path");
        }
        struct stat dotDotStat;
        if (::stat("..", &dotDotStat) < 0)
        {
            auto savedErrno = errno;
            throw std::system_error(savedErrno, std::generic_category(),
                                    "cannot stat a directory in the path");
        }
        if (dotStat.st_ino == dotDotStat.st_ino) // root directory
        {
            break;
        }

        auto dotDotDir = ::opendir("..");
        if (!dotDotDir)
        {
            auto savedErrno = errno;
            throw std::system_error(savedErrno, std::generic_category(),
                                    "cannot open a directory in the path");
        }
        // iterate over the parent directory to find the name of the current
        //  directory. if not found, iterate again and stat() each entry, to get
        //  the 'inner' inode of mount points.
        for (auto stat = false;;)
        {
            errno = 0; // to distinguish end-of-directory from error
            auto ent = ::readdir(dotDotDir);
            if (!ent)
            {
                if (errno != 0)
                {
                    auto savedErrno = errno;
                    throw std::system_error(savedErrno, std::generic_category(),
                                            "cannot read a directory in the path");
                }
                if (stat)
                {
                    throw std::logic_error(
                            "parent directory doesn't contain its subdirectory");
                }
                // iterate again and stat()
                stat = true;
                ::rewinddir(dotDotDir);
                continue;
            }
            if (stat)
            {
                struct stat entStat;
                if (::fstatat(::dirfd(dotDotDir), ent->d_name, &entStat, 0) < 0
                        || entStat.st_ino != dotStat.st_ino
                        || entStat.st_dev != dotStat.st_dev)
                {
                    continue;
                }
            }
            else
            {
                if (ent->d_ino != dotStat.st_ino)
                {
                    continue;
                }
            }
            path.push_back(ent->d_name);
            break;
        }
        if (::fchdir(::dirfd(dotDotDir)) < 0)
        {
            auto savedErrno = errno;
            throw std::system_error(savedErrno, std::generic_category(),
                                    "cannot change to parent directory");
        }
        (void) ::closedir(dotDotDir);
    }
    for (auto i = path.rbegin(); i != path.rend(); ++i)
    {
        std::cout << '/' << *i;
    }
    std::cout << '\n';
}
catch (const std::exception& ex)
{
    std::cerr << argv[0] << ": " << ex.what() << '\n';
    return 1;
}
catch (...)
{
    std::cerr << argv[0] << ": unhandled unknown exception\n";
    return 1;
}
