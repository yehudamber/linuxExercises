#include "controller.h"

#include "directory.h"
#include "utils.h"

#include <exception>
#include <iomanip>
#include <ostream>
#include <stdexcept>

#include <unistd.h>

using namespace std::string_view_literals;

Controller::Controller(
                int argc, char* argv[], std::ostream& out, std::ostream& err)
    : m_args(argc, argv), m_out(out), m_err(err)
{
    m_instance = this; // assume that only one controller is
                       // created at the same time
}

Controller::~Controller()
{
    m_instance = nullptr;
}

int Controller::run()
{
    if (m_args.m_wasError)
    {
        m_err << "Try '" << m_args.m_progName << " -h' for more information.\n";
        return ExitStatus::CMDLINEERR;
    }

    if (m_args.m_showHelp)
    {
        m_err << "Usage: " << m_args.m_progName << " [-hr] [DIRECTORY]...\n"
                 "Print the content of the DIRECTORY(ies).\n\n"
                 "With no DIRECTORY, use the current working directory.\n"
                 "Exit status is 0 for success, 1 for failure and 2 for"
                 " command-line error.\n\n"
                 "  -h\tShow this help and exit\n"
                 "  -r\tAlso print the content of subdirectories\n";
        return ExitStatus::SUCCESS;
    }

    if (m_args.m_positional.empty())
    {
        process("."sv);
    }
    else
    {
        for (auto path : m_args.m_positional)
        {
            process(path);
        }
    }
    return m_exitStatus;
}

std::ostream& Controller::error()
{
    m_instance->m_exitStatus = ExitStatus::FAILURE;
    return m_instance->m_err << m_instance->m_args.m_progName << ": ";
}

void Controller::process(const std::string_view& path, bool isCmdLineArg) try
{
    constexpr auto inodeFieldWidth     = 8;
    constexpr auto linkCountFieldWidth = 3;
    constexpr auto unknown             = "<?>";

    auto dir = Directory(path);
    while (auto ent = dir.next())
    {
        if (!isCmdLineArg)
        {
            m_out << '\t';
        }
        if (ent->m_extra)
        {
            m_out << std::setw(inodeFieldWidth) << ent->m_extra->m_inode << ' '
                  << fmtMode(ent->m_extra->m_mode) << ' '
                  << std::setw(linkCountFieldWidth) << ent->m_extra->m_linkCount
                  << ' ';
        }
        else
        {
            m_out << std::setw(inodeFieldWidth)     << unknown << ' '
                  << std::setw(modeStringLen)       << unknown << ' '
                  << std::setw(linkCountFieldWidth) << unknown << ' ';
        }
        m_out << ent->m_name;
        if (ent->m_linkTarget)
        {
            m_out << " -> " << *ent->m_linkTarget;
        }
        m_out << '\n';

        if (isCmdLineArg && m_args.m_readSubdirs && ent->isSubdir()
                && ent->m_name != "." && ent->m_name != "..")
        {
            process(std::string(path) + '/' + ent->m_name, false);
        }
    }
}
catch (const std::exception& ex)
{
    error() << ex.what() << '\n';
}
catch (...)
{
    error() << "Unknown exception\n";
}

Controller::Arguments::Arguments(int argc, char* argv[]) : m_progName(argv[0])
{
    for (int opt; (opt = ::getopt(argc, argv, "hr")) != -1;)
    {
        switch (opt)
        {
        case 'h':
            m_showHelp = true;
            break;
        case 'r':
            m_readSubdirs = true;
            break;
        case '?':
            m_wasError = true;
            return;
        default:
            throw std::logic_error("unexpected result from getopt()");
        }
    }
    m_positional = {argv + ::optind, argv + argc};
}
