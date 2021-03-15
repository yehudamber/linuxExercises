#include <array>
#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <span>
#include <string>
#include <string_view>
#include <system_error>

#include <fcntl.h>
#include <unistd.h>

using namespace std::string_view_literals;

std::string_view progName;

class File
{
public:
    explicit File(const std::string_view& path, int flags, mode_t mode = 0)
        : m_path(path), m_fd(::open(m_path.c_str(), flags, mode))
    {
        if (m_fd < 0)
        {
            throw std::system_error(errno, std::generic_category(),
                                    "failed to open " + m_path);
        }
    }

    explicit File(int fd, const std::string_view& path)
        : m_path(path), m_fd(fd), m_needToClose(false)
    {
    }

    File(File&&)            = delete; // this will delete the copy constructor
    File& operator=(File&&) = delete; //    and assignment operator

    ~File()
    {
        if (m_needToClose && ::close(m_fd) < 0)
        {
            // cannot throw an exception from a destructor
            std::cerr << progName << ": failed to close " << m_path << ": "
                      << std::generic_category().message(errno) << '\n';
        }
    }

    template <auto N>
    auto read(std::array<std::byte, N>& buf)
    {
        if (auto count = ::read(m_fd, buf.data(), N); count < 0)
        {
            throw std::system_error(errno, std::generic_category(),
                                    "failed to read from " + m_path);
        }
        else
        {
            return static_cast<std::size_t>(count);
        }
    }

    void write(const std::span<std::byte>& buf)
    {
        if (::write(m_fd, buf.data(), buf.size()) < std::ssize(buf))
        {
            throw std::system_error(errno, std::generic_category(),
                                    "failed to write to " + m_path);
        }
    }

private:
    std::string m_path;
    int m_fd;
    bool m_needToClose = true;
};

constexpr auto openSrcFile  = O_RDONLY;
constexpr auto openDestFile = O_WRONLY | O_TRUNC | O_CREAT;
constexpr auto destFileMode = S_IRUSR | S_IWUSR
                            | S_IRGRP | S_IWGRP
                            | S_IROTH | S_IWOTH; // 0666

constexpr auto stdStreamPath = "-"sv;

int main(int argc, char* argv[]) try
{
    progName = argv[0];

    if (argc != 3)
    {
        std::cerr << "usage: " << progName << " SOURCE DEST\n";
        return EXIT_FAILURE;
    }

    auto srcPath  = std::string_view(argv[1]);
    auto destPath = std::string_view(argv[2]);
    auto srcFile  = srcPath == stdStreamPath
                        ? File(STDIN_FILENO, "<stdin>"sv)
                        : File(srcPath, openSrcFile);
    auto destFile = destPath == stdStreamPath
                        ? File(STDOUT_FILENO, "<stdout>"sv)
                        : File(destPath, openDestFile, destFileMode);

    auto buf = std::array<std::byte, BUFSIZ>();
    while (auto count = srcFile.read(buf))
    {
        destFile.write({buf.data(), count});
    }
}
catch (const std::exception& ex)
{
    std::cerr << progName << ": " << ex.what() << '\n';
    return EXIT_FAILURE;
}
catch (...)
{
    std::cerr << progName << ": Unkonwn exception\n";
    return EXIT_FAILURE;
}
