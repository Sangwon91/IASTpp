// Qt Libraries
#include <QApplication>
#include <QSharedPointer>

// STD Libraries
#include <exception>
#include <iostream>

// User-Defined Libraries
#include "iast_gui.h"
#include "iast_gui_logger.h"

int
main(int argc, char* argv[])
try {
    IAST_GUI_LOG("Start log");

    QApplication app(argc, argv);

    new IastGui;

    return app.exec();
    }
catch(std::exception& e)
    {
    std::cerr << e.what() << std::endl;

    return 1;
    }
