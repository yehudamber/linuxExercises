/***********************************************
 * directory.h - Definision of class Directory *
 ***********************************************/

#pragma once

#include <string>
#include <string_view>
#include <optional>

#include <dirent.h>

/*
 * Directory - a wrapper class for the POSIX directory reading interface
 */

class Directory
{
public:
    struct Entry
    {
        ino_t m_inode;
        std::string m_name;
    };

    explicit Directory(const std::string_view& path);
    ~Directory();
    Directory(const Directory&)            = delete;
    Directory& operator=(const Directory&) = delete;
    std::optional<Entry> next(); // returns std::nullopt for end-of-directory

private:
    std::string m_path;
    DIR* m_dir;
};
