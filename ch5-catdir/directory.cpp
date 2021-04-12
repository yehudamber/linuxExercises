#include "directory.h"

#include "controller.h"

#include <cerrno>
#include <ostream>
#include <system_error>
#include <utility>

#include <sys/stat.h>
#include <unistd.h>

Directory::Directory(const std::string_view& path)
    : m_path(path), m_dir(::opendir(m_path.c_str()))
{
    if (!m_dir)
    {
        auto savedErrno = errno;
        throw std::system_error(savedErrno, std::generic_category(),
                                "failed to open " + m_path);
    }
}

Directory::~Directory()
{
    if (::closedir(m_dir) < 0)
    {
        // cannot throw an exception from a destructor
        auto savedErrno = errno;
        Controller::error() << "failed to close " << m_path << ": "
                            << std::generic_category().message(savedErrno)
                            << '\n';
    }
}

std::optional<Directory::Entry> Directory::next()
{
    errno = 0; // to distinguish end of directory from an error
    if (auto ent = ::readdir(m_dir))
    {
        return entryFor(ent->d_name);
    }
    if (errno == 0) // end of directory
    {
        return std::nullopt;
    }
    auto savedErrno = errno;
    throw std::system_error(savedErrno, std::generic_category(),
                            "failed to read entry from " + m_path);
}

namespace
{

/*
 * readlink - reads the content of the symlink 'path', using initial buffer of
 *            size 'size'
 *
 * We can't rely on the size returned by lstat, because the link can increase
 * between tha calls, and because the kernel doesn't give a size for the
 * symlinks on pseudo-filesystems such as /proc, so we need to iterate until the
 * buffer fits the link content.
 *
 * The code main layout is based on the GNU coreutils v5.0 xreadlink function.
 */

std::optional<std::string> readlink(const std::string& path, off_t size)
{
    if (size == 0) // the OS doesn't give us the size, so we need to guess
    {
        size = 80;
    }
    auto res = std::string();
    while (true)
    {
        res.resize(size + 1); // one additional character for the terminating
                              // null
        if (auto readCount = ::readlink(path.c_str(), res.data(), size + 1);
            readCount < 0)
        {
            auto savedErrno = errno;
            Controller::error() << "failed to read link " << path << ": "
                                << std::generic_category().message(savedErrno)
                                << '\n';
            return std::nullopt;
        }
        else if (readCount <= size)
        {
            res.resize(readCount);
            return res;
        }
        size *= 2;
    }
}

}

Directory::Entry Directory::entryFor(std::string name)
{
    auto res = Entry{ std::move(name), std::nullopt, std::nullopt };

    auto path = m_path + '/' + res.m_name;

    struct stat data;
    if (::lstat(path.c_str(), &data) < 0)
    {
        auto savedErrno = errno;
        Controller::error() << "failed to stat " << path << ": "
                            << std::generic_category().message(savedErrno)
                            << '\n';
        return res;
    }

    res.m_extra = { data.st_ino, data.st_mode, data.st_nlink };
    if (S_ISLNK(data.st_mode))
    {
        res.m_linkTarget = readlink(path, data.st_size);
    }
    return res;
}
