#pragma once

#include <exception>
#include <string>

class IastException : public std::exception
    {
public:
    IastException(const char* file, const int& line, const std::string& msg);

    virtual const char* what() const noexcept override;
private:
    std::string mMessage;
    };
