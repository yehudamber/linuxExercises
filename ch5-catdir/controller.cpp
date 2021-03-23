/*******************************************************
 * controller.cpp - Implementation of class Controller *
 *******************************************************/

#include "controller.h"

#include "directory.h"

#include <exception>
#include <iomanip>
#include <ostream>

using namespace std::string_view_literals;

Controller::Controller(
                int argc, char* argv[], std::ostream& out, std::ostream& err)
    : // argv[0] is used only as the program name
      m_progName(argv[0]), m_argc(argc - 1), m_argv(argv + 1),
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
    if (m_argc == 0)
    {
        process("."sv);
    }
    else
    {
        for (auto i = 0; i < m_argc; ++i)
        {
            process(m_argv[i]);
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
    auto dir = Directory(path);
    while (auto ent = dir.next())
    {
        m_out << std::setw(8) << ent->m_inode << ' ' << ent->m_name << '\n';
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

Controller* Controller::m_instance;
