/*************************************************
 * controller.h - Definision of class Controller *
 *************************************************/

#pragma once

#include <cstdlib>
#include <iosfwd>
#include <string_view>

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

    static Controller* m_instance; // static pointer to the created controller

    const std::string_view m_progName;
    const int m_argc;
    const char* const* const m_argv;
    std::ostream& m_out;
    std::ostream& m_err;
    int m_exitStatus = EXIT_SUCCESS;
};
