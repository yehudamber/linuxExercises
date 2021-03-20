#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>

#include <unistd.h>

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "usage: " << argv[0] << " FILE LENGTH\n";
        return EXIT_FAILURE;
    }
    auto length = off_t();
    if (!(std::istringstream(argv[2]) >> length))
    {
        std::cerr << argv[0] << ": invalid length: " << argv[2] << '\n';
        return EXIT_FAILURE;
    }
    if (::truncate(argv[1], length) < 0)
    {
        std::cerr << argv[0] << ": failed to truncate " << argv[1] << " at "
                  << length << " bytes: " << std::strerror(errno) << '\n';
        return EXIT_FAILURE;
    }
}
