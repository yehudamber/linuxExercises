#include <readline.h>

#include <cstdlib>
#include <exception>
#include <iostream>

int main() try
{
    Line line;
    line.m_fp = stdin;
    while (line.read() != -1)
    {
        std::cout << '(' << line.m_buflen << ") " << line.m_buf << '\n';
    }
}
catch (const std::exception& exception)
{
    std::cerr << exception.what() << '\n';
    return EXIT_FAILURE;
}
catch (...)
{
    std::cerr << "Unknown exception\n";
    return EXIT_FAILURE;
}
