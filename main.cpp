// Qt Libraries
#include <QApplication>
#include <QSharedPointer>

// STD Libraries
#include <exception>
#include <iostream>

// User-Defined Libraries
#include "iast_gui.h"

int
main(int argc, char* argv[])
try {
    QApplication app(argc, argv);

    new IastGui;

    return app.exec();
    }
catch(std::exception& e)
    {
    std::cerr << e.what() << std::endl;

    return 1;
    }
