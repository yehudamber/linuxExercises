#include <iostream>

int main(int argc, char** argv)
{
    const auto progName = *(argv++);
    --argc;

    auto aFlg = false;
    auto cFlg = false;
    const char* bParam = nullptr;

    for (; *argv; --argc, ++argv)
    {
        if (**argv != '-')
        {
            break;
        }
        if (*(++*argv) == '-')
        {
            --argc;
            ++argv;
            break;
        }
        for (; **argv; ++*argv)
        {
            switch (**argv)
            {
            case 'a':
                aFlg = true;
                continue;
            case 'c':
                cFlg = true;
                continue;
            case 'b':
                if (*(++*argv))
                {
                    bParam = *argv;
                }
                else
                {
                    if (argv[1])
                    {
                        --argc;
                        bParam = *++argv;
                    }
                    else
                    {
                        std::cerr << progName
                                  << ": option `-b' requires an argument\n";
                    }
                }
                break;
            default:
                std::cerr << progName << ": option `-" << **argv
                          << "' is invalid: ignored\n";
                continue;
            }
            break;
        }
    }

    std::cout << std::boolalpha
              << "aFlg = " << aFlg << "\n"
                 "cFlg = " << cFlg << "\n"
                 "bParam = \"" << (bParam ? bParam : "") << "\"\n"
                 "argc = " << argc << '\n';
    for (auto i = 0; i < argc; ++i)
    {
        std::cout << "argv[" << i << "] = \"" << argv[i] << "\"\n";
    }
}
