#include <iostream>
#include <iomanip>
#include <memory>
#include <fstream>
#include "../../isotherm.hpp"
#include "../../isotherm_factory.hpp"
#include "../../isotherm_exception.hpp"
#include "../../isotherm_utility.hpp"
#include "../../interpolator_isotherm.hpp"
#include "../../bet_isotherm.hpp"
using namespace std;

int
main(int argc, char* argv[])
try {
    IsothermFactory factory;
    shared_ptr<Isotherm> iso = factory.create("lang.iso");
    cout << "0." << endl;
    cout << iso->getInfoString() << endl;

    iso = factory.create("lf.iso");
    cout << "1." << endl;
    cout << iso->getInfoString() << endl;

    iso = factory.create("bet.iso");
    cout << "2." << endl;
    cout << iso->getInfoString() << endl;

    try {
        iso = factory.create("dsl.iso");
        cout << "3." << endl;
        cout << iso->getInfoString() << endl;
        }
    catch (IsothermException& e)
        {
        cout << e.what() << endl;
        }

    iso = factory.create("inter.iso");
    cout << "4." << endl;
    cout << iso->getInfoString() << endl;

    iso = factory.create("qua.iso");
    cout << "5." << endl;
    cout << iso->getInfoString() << endl;

    iso = factory.create("dslf.iso");
    cout << "6." << endl;
    cout << iso->getInfoString() << endl;

    iso = factory.create("henry.iso");
    cout << "7." << endl;
    cout << iso->getInfoString() << endl;

    iso = factory.create("dsl2.iso");
    cout << "8." << endl;
    cout << iso->getInfoString() << endl;

    auto info = iso->getInfoString();

    ofstream ofs {"outtest.iso"};
    ofs << info;
    ofs.close();

    iso = factory.create("outtest.iso");

    cout << "9." << endl;
    cout << iso->getInfoString() << endl;

    return 0;
    }
catch (exception& e)
    {
    cout << e.what() << endl;
    }
