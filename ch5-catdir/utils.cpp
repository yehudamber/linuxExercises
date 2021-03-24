#include "utils.h"

#include <cctype>

#include <sys/stat.h>

namespace
{

auto typeChar(mode_t mode)
{
    if (S_ISREG(mode))  { return '-'; }
    if (S_ISDIR(mode))  { return 'd'; }
    if (S_ISLNK(mode))  { return 'l'; }
    if (S_ISCHR(mode))  { return 'c'; }
    if (S_ISBLK(mode))  { return 'b'; }
    if (S_ISFIFO(mode)) { return 'p'; }
    if (S_ISSOCK(mode)) { return 's'; }
    return '?';
}

void setAdditionalFlag(char& target, char flag)
{
    target = target == '-' ? std::toupper(static_cast<unsigned char>(flag))
                           : flag;
}

}

std::string fmtMode(mode_t mode)
{
    auto res = std::string(10, '-');
    res[0] = typeChar(mode);

    if (mode & S_IRUSR) { res[1] = 'r'; }
    if (mode & S_IWUSR) { res[2] = 'w'; }
    if (mode & S_IXUSR) { res[3] = 'x'; }
    if (mode & S_IRGRP) { res[4] = 'r'; }
    if (mode & S_IWGRP) { res[5] = 'w'; }
    if (mode & S_IXGRP) { res[6] = 'x'; }
    if (mode & S_IROTH) { res[7] = 'r'; }
    if (mode & S_IWOTH) { res[8] = 'w'; }
    if (mode & S_IXOTH) { res[9] = 'x'; }

    if (mode & S_ISUID)
    {
        setAdditionalFlag(res[3], 's');
    }
    if (mode & S_ISGID)
    {
        setAdditionalFlag(res[6], 's');
    }
    if (mode & S_ISVTX)
    {
        setAdditionalFlag(res[9], 't');
    }
    return res;
}
