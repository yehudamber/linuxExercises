#include "directory.h"

#include "controller.h"

#include <cerrno>
#include <ostream>
#include <system_error>
#include <utility>

#include <sys/stat.h>

Directory::Directory(const std::string_view& path)
    : m_path(path), m_dir(::opendir(m_path.c_str()))
{
    if (!m_dir)
    {
        throw std::system_error(errno, std::generic_category(),
                                "failed to open " + m_path);
    }
}

Directory::~Directory()
{
    if (::closedir(m_dir) < 0)
    {
        // cannot throw an exception from a destructor
        Controller::error() << "failed to close " << m_path << ": "
                            << std::generic_category().message(errno) << '\n';
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
    throw std::system_error(errno, std::generic_category(),
                            "failed to read entry from " + m_path);
}

Directory::Entry Directory::entryFor(std::string name)
{
    auto res = Entry{ std::move(name), std::nullopt };

    auto path = m_path + '/' + res.m_name;

    struct stat data;
    if (::stat(path.c_str(), &data) < 0)
    {
        Controller::error() << "failed to stat " << path << ": "
                            << std::generic_category().message(errno) << '\n';
        return res;
    }

    res.m_extra = { data.st_ino, data.st_mode, data.st_nlink };
    return res;
}
