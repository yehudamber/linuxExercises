#pragma once

#include <cstdio>
#include <memory>

struct Line
{
    std::size_t m_buflen = 0;
    std::unique_ptr<char[]> m_buf;
    std::FILE* m_fp = nullptr;

    int read();

private:
    void realloc();
};
