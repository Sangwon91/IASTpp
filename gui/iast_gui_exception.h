#ifndef IASTGUIEXCEPTION_H
#define IASTGUIEXCEPTION_H

#include <string>
#include <exception>

class IastGuiException : std::exception
{
public:
    IastGuiException(const std::string& file, int line, const std::string& msg);
    virtual const char* what() const noexcept override;
private:
    std::string mMsg;
};

#define throw_gui_exception(...) \
    throw IastGuiException {__FILE__, __LINE__, __VA_ARGS__};

#endif // IASTGUIEXCEPTION_H
