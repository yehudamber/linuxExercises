#pragma once

#include <iosfwd>
#include <span>
#include <string_view>

/*
 * ExitStatus - constants for the exit status values returned by the program
 */

namespace ExitStatus
{

static constexpr auto SUCCESS    = 0;
static constexpr auto FAILURE    = 1;
static constexpr auto CMDLINEERR = 2;

}

/*
 * Controller - the class that control the execution of the program
 */

class Controller
{
public:
    Controller(int argc, char* argv[], std::ostream& out, std::ostream& err);
    ~Controller();
    Controller(const Controller&)            = delete;
    Controller& operator=(const Controller&) = delete;
    int run();

    static std::ostream& error();

private:
    void process(const std::string_view& path, bool isCmdLineArg = true);

    // static pointer to the created controller
    static inline Controller* m_instance = nullptr;

    const struct Arguments
    {
        std::string_view m_progName;
        std::span<const char* const> m_positional;
        bool m_showHelp    = false;
        bool m_readSubdirs = false;
        bool m_wasError    = false;

        explicit Arguments(int argc, char* argv[]);
    } m_args;
    std::ostream& m_out;
    std::ostream& m_err;
    int m_exitStatus = ExitStatus::SUCCESS;
};
