// Qt Libraries
#include <QApplication>
#include <QSharedPointer>

// STD Libraries
#include <exception>
#include <iostream>

// User-Defined Libraries
#include "gui/iast_gui.h"
#include "gui/iast_gui_logger.h"

int
main(int argc, char* argv[])
try {
    IAST_GUI_LOG("Start Program");
    IAST_GUI_LOG("argv[0]:", argv[0]);
    QApplication app(argc, argv);

    new IastGui;

    return app.exec();
    }
catch(std::exception& e)
    {
    IAST_GUI_LOG("Unexpected exception occurs");
    std::cerr << e.what() << std::endl;

    return 1;
    }
