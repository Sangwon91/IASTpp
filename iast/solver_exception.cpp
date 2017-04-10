#include "solver_exception.hpp"

SolverException::SolverException(
    const char* file,
    const int&  line,
    const std::string& msg) : mMessage {}
    {
    mMessage += "File: ";
    mMessage += file;
    mMessage += ", Line: ";
    mMessage += std::to_string(line);
    mMessage += ": " + msg;
    }

const char*
SolverException::what() const noexcept
    {
    return mMessage.c_str();
    }
