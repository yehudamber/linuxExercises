#include "controller.h"

#include <iostream>

int main(int argc, char* argv[])
{
    return Controller(argc, argv, std::cout, std::cerr).run();
}
