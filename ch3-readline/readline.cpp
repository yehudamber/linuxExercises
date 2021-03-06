#include "readline.h"

#include <algorithm>
#include <cerrno>
#include <system_error>

constexpr auto initialBufLen = 80;

int Line::read()
{
    if (!m_fp)
    {
        return -1;
    }

    if (!m_buf)
    {
        m_buflen = initialBufLen;
        m_buf = std::make_unique<char[]>(m_buflen);
    }

    std::size_t len = 0;
    while (true)
    {
        for (; len < m_buflen; ++len)
        {
            switch (auto ch = std::fgetc(m_fp))
            {
            case '\n':
                if (len > 0 && m_buf[len - 1] == '\r') // CR-LF line termination
                {
                    --len; // the null will overwrite the '\r'
                }
                m_buf[len] = '\0';
                return 1;

            case EOF:
                m_buf[len] = '\0';
                if (std::ferror(m_fp))
                {
                    throw std::system_error(errno, std::generic_category(),
                                            "fgetc failed");
                }
                return len == 0 ? -1 : 1;

            default:
                m_buf[len] = ch;
                break;
            }
        }
        realloc();
    }
}

void Line::realloc()
{
    auto newLen = m_buflen * 2;
    auto newBuf = std::make_unique<char[]>(newLen);
    std::copy_n(m_buf.get(), m_buflen, newBuf.get());
    m_buflen = newLen;
    m_buf    = std::move(newBuf);
}
