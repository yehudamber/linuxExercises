#include <iostream>

#include <getopt.h>

int main(int argc, char* argv[])
{
    for (int opt; (opt = getopt(argc, argv, ":ab:c")) != -1;)
    {
        switch (opt)
        {
        case 'a':
        case 'b':
        case 'c':
            break;
        case '?':
            std::cerr << argv[0] << ": What is '-" << static_cast<char>(optopt)
                      << "'?\n";
            break;
        case ':':
            std::cerr << argv[0] << ": '-" << static_cast<char>(optopt)
                      << "' what?\n";
            break;
        default:
            return -1;
        }
    }
}
