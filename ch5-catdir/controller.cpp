#include "controller.h"

#include "directory.h"
#include "utils.h"

#include <exception>
#include <iomanip>
#include <ostream>

using namespace std::string_view_literals;

Controller::Controller(
                int argc, char* argv[], std::ostream& out, std::ostream& err)
    : // argv[0] is used only as the program name
      m_progName(argv[0]), m_args(argv + 1, argc - 1),
      m_out(out), m_err(err)
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
    if (m_args.empty())
    {
        process("."sv);
    }
    else
    {
        for (auto path : m_args)
        {
            process(path);
        }
    }
    return m_exitStatus;
}

std::ostream& Controller::error()
{
    m_instance->m_exitStatus = EXIT_FAILURE;
    return m_instance->m_err << m_instance->m_progName << ": ";
}

void Controller::process(const std::string_view& path) try
{
    constexpr auto inodeFieldWidth     = 8;
    constexpr auto linkCountFieldWidth = 3;
    constexpr auto unknown             = "<?>";

    auto dir = Directory(path);
    while (auto ent = dir.next())
    {
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
