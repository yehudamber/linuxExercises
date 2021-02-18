#include <iostream>

extern char** environ;

int main(int /*argc*/, char* /*argv*/[], char** envp)
{
    std::cout << std::boolalpha
              << "environ == envp: " << (environ == envp) << '\n';
}
