#include "iast_gui_exception.h"

#include <sstream>

IastGuiException::IastGuiException(const std::string& file, int line, const std::string& msg)
    {
    std::stringstream ss;
    ss << "File: " << file << ", Line: " << line << ", Msg: " << msg;
    mMsg = ss.str();
    }

const char*
IastGuiException::what() const noexcept
    {
    return mMsg.c_str();
    }
