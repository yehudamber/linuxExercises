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
        std::string m_name;

        struct ExtraData
        {
            ino_t m_inode;
            mode_t m_mode;
            nlink_t m_linkCount;
        };
        std::optional<ExtraData> m_extra;
        std::optional<std::string> m_linkTarget;

        bool isSubdir() const;
    };

    explicit Directory(const std::string_view& path);
    ~Directory();
    Directory(const Directory&)            = delete;
    Directory& operator=(const Directory&) = delete;
    std::optional<Entry> next(); // returns std::nullopt for end-of-directory

private:
    Entry entryFor(std::string name);

    std::string m_path;
    DIR* m_dir;
};
