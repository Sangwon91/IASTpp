#ifndef IAST_GUI_LOGGER_H
#define IAST_GUI_LOGGER_H

#include <memory>
#include <string>
#include <iostream>

class IastGuiLogger
{
public:
    static IastGuiLogger& instance();

    template <typename... Arguments>
    void log(const std::string& file,
             const int& line,
             const Arguments&... args);

    virtual ~IastGuiLogger();
protected:
    static std::unique_ptr<IastGuiLogger> mInstance;
private:
    IastGuiLogger();

    template <typename T>
    void log_helper(std::ostream& os, const T& msg);

    template <typename First, typename... Rest>
    void log_helper(std::ostream& os,
                    const First& first,
                    const Rest&... rest);

    //template <typename... Arguments>
    //void log_helper(std::ostream& os, const Arguments&... args);
};

// Template member functions are defined in header.
template <typename... Arguments>
void
IastGuiLogger::log(const std::string &file,
                   const int &line,
                   const Arguments&... args)
    {
    std::ostream& os = std::clog;

    os << "FILE: " << file << ", " <<
          "LINE: " << line << ", ";

    log_helper(os, args...);
    }

template <typename T>
void
IastGuiLogger::log_helper(std::ostream& os, const T& msg)
    {
    os << msg << std::endl;
    }

template <typename First, typename... Rest>
void
IastGuiLogger::log_helper(std::ostream& os, const First& first, const Rest&... rest)
    {
    os << first << ' ';

    log_helper(os, rest...);
    }

// Helper macro
#define IAST_GUI_LOG(...) \
    IastGuiLogger::instance().log(__FILE__, __LINE__, __VA_ARGS__);

#endif // IAST_GUI_LOGGER_H
