#include <iostream>

#include <getopt.h>

int main(int argc, char* argv[])
{
    auto aFlg = false;
    auto cFlg = false;
    const char* bParam = nullptr;
    auto res = 0;

    for (int opt; (opt = getopt(argc, argv, "ab:c")) != -1;)
    {
        switch (opt)
        {
        case 'a':
            aFlg = true;
            break;
        case 'b':
            bParam = optarg;
            break;
        case 'c':
            cFlg = true;
            break;
        case '?':
        default:
            res = 1;
            break;
        }
    }

    std::cout << std::boolalpha
              << "aFlg = " << aFlg << "\n"
                 "cFlg = " << cFlg << "\n"
                 "bParam = ";
    if (bParam)
    {
        std::cout << '"' << bParam << "\"\n";
    }
    else
    {
        std::cout << "<null>\n";
    }
    for (auto i = optind; i < argc; ++i)
    {
        std::cout << "argv[" << i << "] = \"" << argv[i] << "\"\n";
    }

    return res;
}
