#include <iostream>

#include <getopt.h>

int main(int argc, char* argv[])
{
    opterr = 0;
    for (int opt; (opt = getopt(argc, argv, "ab:c")) != -1;)
    {
        switch (opt)
        {
        case 'a':
        case 'b':
        case 'c':
            break;
        case '?':
            std::cerr << argv[0] << ": option '-" << static_cast<char>(optopt)
                      << "' is invalid\n";
            break;
        default:
            return -1;
        }
    }
}
