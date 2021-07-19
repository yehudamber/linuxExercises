#pragma once

#include <iosfwd>
#include <span>
#include <string_view>

/*
 * ExitStatus - constants for the exit status values returned by the program
 */

namespace ExitStatus
{

static constexpr auto SUCCESS = 0;
static constexpr auto FAILURE = 1;

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
    void process(const std::string_view& path);

    // static pointer to the created controller
    static inline Controller* m_instance = nullptr;

    const std::string_view m_progName;
    const std::span<const char* const> m_args;
    std::ostream& m_out;
    std::ostream& m_err;
    int m_exitStatus = ExitStatus::SUCCESS;
};
