#pragma once

#include <exception>
#include <string>

class SolverException : public std::exception
    {
public:
    SolverException(const char* file, const int& line, const std::string& msg);

    virtual const char* what() const noexcept override;
private:
    std::string mMessage;
    };

class NoRootException : public SolverException
    {
    using SolverException::SolverException;
    };
