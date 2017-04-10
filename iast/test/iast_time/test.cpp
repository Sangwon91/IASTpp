#include <iostream>
#include <iomanip>
using namespace std;

#include "iast.hpp"
#include "isotherm_factory.hpp"

int
main()
try {
    IsothermFactory factory;

    Iast::IsothermVector isotherms (2);
    isotherms[0] = factory.create("ch4.dat");
    isotherms[1] = factory.create("c2h6.dat");

    cout << isotherms[0]->getInfoString() << endl;
    cout << isotherms[1]->getInfoString() << endl;

    Iast::Mode mode = Iast::Mode::FIX_PY;
    double pressure = 65.0;
    vector<double> composition {0.95, 0.05};

    Iast iast;
    iast.setIsotherms(isotherms);

    double uptake = 0.0;
    vector<double> x;

    for (int i = 0; i < 10000; ++i)
        {
        iast.calculate(mode, pressure, composition);
        tie(uptake, x) = iast.getResult();
        }

    for (auto a : x)
        cout << a * uptake << ", ";
    cout << endl;

    return 0;
    }
catch (exception& e)
    {
    cout << e.what() << endl;
    }
