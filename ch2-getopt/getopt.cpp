#include "getopt.h"

#include <cstring>
#include <iostream>

inline namespace MyUtils
{

constexpr auto OPTCHAR    = '-';
constexpr auto INVALIDOPT = '?';
constexpr auto NEEDARG    = ':';

int getopt(int argc, char* const argv[], const char* optstring)
{
    static auto innerInd = 0;
    const auto useNeedArgErrs = optstring[0] == NEEDARG;
    const auto printErrs      = opterr && !useNeedArgErrs;
    if (innerInd == 0)
    {
        if (optind >= argc || argv[optind][0] != OPTCHAR || !argv[optind][1])
        {
            // it is not an option, stop processing
            return -1;
        }
        if (argv[optind][1] == OPTCHAR && !argv[optind][2])
        {
            // it is a '--', skip it and stop processing
            ++optind;
            return -1;
        }
        ++innerInd; // skip the '-'
    }
    auto opt = argv[optind][innerInd++];
    optopt = opt;
    if (!argv[optind][innerInd]) // end of word
    {
        ++optind;
        innerInd = 0;
    }
    if (auto found = std::strchr(optstring, opt)) // it is a valid option
    {
        if (found[1] == NEEDARG)
        {
            if (optind >= argc) // there is no argument
            {
                if (printErrs)
                {
                    std::cerr << argv[0] << ": option requires an argument -- '"
                              << opt << "'\n";
                }
                return useNeedArgErrs ? NEEDARG : INVALIDOPT;
            }
            optarg = argv[optind++] + innerInd; // handles correctly both
            innerInd = 0;                       // '-oARG' and '-o ARG'
        }
        return opt;
    }
    if (printErrs)
    {
        std::cerr << argv[0] << ": invalid option -- '" << opt << "'\n";
    }
    return INVALIDOPT;
}

char* optarg = nullptr;

int optind = 1; // skip argv[0]
int optopt = 0;
int opterr = 1;

} // inline namespace MyUtils
