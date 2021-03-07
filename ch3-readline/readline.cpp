#include "readline.h"

#include <algorithm>
#include <cerrno>
#include <cstring>
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
        if (!std::fgets(m_buf.get() + len, m_buflen - len, m_fp))
        {
            if (std::ferror(m_fp))
            {
                throw std::system_error(errno, std::generic_category(),
                                        "fgets failed");
            }
            return len == 0 ? -1 : 1;
        }

        len += std::strlen(m_buf.get() + len);
        if (m_buf[len - 1] == '\n')
        {
            --len;
            if (len > 0 && m_buf[len - 1] == '\r') // CR-LF line termination
            {
                --len;
            }
            m_buf[len] = '\0';
            return 1;
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
