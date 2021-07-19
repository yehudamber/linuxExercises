#include "controller.h"

#include <exception>
#include <iostream>

int main(int argc, char* argv[]) try
{
    return Controller(argc, argv, std::cout, std::cerr).run();
}
catch (const std::exception& ex)
{
    std::cerr << argv[0] << ": Unhandled exception: " << ex.what() << '\n';
    return ExitStatus::FAILURE;
}
catch (...)
{
    std::cerr << argv[0] << ": Unhandled unknown exception\n";
    return ExitStatus::FAILURE;
}
