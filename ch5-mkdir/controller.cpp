#include "controller.h"

#include "makedir.h"

#include <exception>
#include <ostream>
#include <stdexcept>

#include <unistd.h>

Controller::Controller(
        int argc, char* argv[], std::ostream& out, std::ostream& err)
    : m_args(argc, argv), m_out(out), m_err(err)
{
}

int Controller::run()
{
    auto cmdLineError = [this]{
        m_err << "Try '" << m_args.m_progName << " -h' for more information.\n";
        return ExitStatus::CMDLINEERR;
    };

    if (m_args.m_wasError)
    {
        return cmdLineError();
    }

    if (m_args.m_showHelp)
    {
        m_out << "Usage: " << m_args.m_progName << " [-h] DIRECTORY...\n"
                 "Create the DIRECTORY(ies).\n\n"
                 "Exit status is 0 for success, 1 for failure and 2 for"
                 " command-line error.\n\n"
                 "  -h\tShow this help and exit\n";
        return ExitStatus::SUCCESS;
    }

    if (m_args.m_positional.empty())
    {
        m_err << m_args.m_progName << ": missing operand\n";
        return cmdLineError();
    }

    for (auto path : m_args.m_positional)
    {
        process(path);
    }
    return m_exitStatus;
}

void Controller::process(const std::string& path) try
{
    makeDir(path);
}
catch (const std::exception& ex)
{
    m_err << m_args.m_progName << ": " << ex.what() << '\n';
    m_exitStatus = ExitStatus::FAILURE;
}

Controller::Arguments::Arguments(int argc, char* argv[]) : m_progName(argv[0])
{
    for (int opt; (opt = ::getopt(argc, argv, "h")) != -1;)
    {
        switch (opt)
        {
        case 'h':
            m_showHelp = true;
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
