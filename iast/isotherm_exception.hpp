#pragma once

#include <exception>
#include <string>

class IsothermException : public std::exception
    {
public:
    IsothermException(const char* file, const int& line, const std::string& msg);

    virtual const char* what() const noexcept override;
private:
    std::string mMessage;
    };
