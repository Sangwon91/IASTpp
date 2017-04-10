#pragma once

#include <exception>
#include <string>

class MinimizerException : public std::exception
    {
public:
    MinimizerException(const char* file, const int& line, const std::string& msg);

    virtual const char* what() const noexcept override;
private:
    std::string mMessage;
    };
