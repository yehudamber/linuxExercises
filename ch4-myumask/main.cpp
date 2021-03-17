#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>

#include <sys/stat.h>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "usage: " << argv[0] << " MASK\n";
        return EXIT_FAILURE;
    }
    auto mask = mode_t();
    if (!(std::istringstream(argv[1]) >> std::oct >> mask))
    {
        std::cerr << argv[0] << ": invalid mask: " << argv[1] << '\n';
        return EXIT_FAILURE;
    }
    if (::umask(mask) < 0)
    {
        std::cerr << argv[0] << ": umask failed: " << std::strerror(errno)
                  << '\n';
        return EXIT_FAILURE;
    }
}
