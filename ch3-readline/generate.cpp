#include <cmath>
#include <iostream>
#include <random>

int main()
{
    auto engine        = std::default_random_engine{std::random_device{}()};
    auto lineSizeDist  = std::normal_distribution<>{100, 25};
    auto characterDist = std::uniform_int_distribution<char>{0x20, 0x7e};

    for (auto i = 0; i < 1000; ++i)
    {
        auto lineSize = std::lround(lineSizeDist(engine));
        if (lineSize < 0)
        {
            lineSize = 0;
        }
        for (auto j = 0; j < lineSize; ++j)
        {
            std::cout << characterDist(engine);
        }
        std::cout << '\n';
    }
}
