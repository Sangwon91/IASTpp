#ifndef IAST_GUI_LOGGER_H
#define IAST_GUI_LOGGER_H

#include <memory>
#include <string>
#include <iostream>

class IastGuiLogger
{
public:
    static IastGuiLogger& instance();
    void log(const std::string& file,
             const int& line,
             const std::string& msg);

    virtual ~IastGuiLogger();
protected:
    static std::unique_ptr<IastGuiLogger> mInstance;
private:
    IastGuiLogger();
};

#define IAST_GUI_LOG(...) \
    IastGuiLogger::instance().log(__FILE__, __LINE__, __VA_ARGS__);

#endif // IAST_GUI_LOGGER_H
