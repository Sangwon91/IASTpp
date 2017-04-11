#include "iast_gui_logger.h"

std::unique_ptr<IastGuiLogger> IastGuiLogger::mInstance {nullptr};

IastGuiLogger::IastGuiLogger()
    {

    }

IastGuiLogger::~IastGuiLogger()
    {

    }

IastGuiLogger&
IastGuiLogger::instance()
    {
    if (!mInstance)
        mInstance = std::unique_ptr<IastGuiLogger> {};

    return *mInstance;
    }

void
IastGuiLogger::log(const std::string &file, const int &line, const std::string& msg)
    {
    std::clog << "FILE: " << file << ", " <<
                 "LINE: " << line << ", " <<
                 msg << std::endl;
    }
