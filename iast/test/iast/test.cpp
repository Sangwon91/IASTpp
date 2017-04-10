#include <iostream>
#include <iomanip>
using namespace std;

#include "../../iast.hpp"
#include "../../langmuir_isotherm.hpp"

int
main()
try {
    cout.setf(ios::left);
    cout << setw(80) << setfill('=') << "IAST TEST START." << endl;

    cout << "    1. isotherm vector & isotherm test    " << endl;
    Iast iast;

    Iast::IsothermVector isotherms;
    isotherms.emplace_back(new LangmuirIsotherm {1.0, 1.0});
    isotherms.emplace_back(new LangmuirIsotherm {1.0, 2.0});

    cout << isotherms[0]->getInfoString() << endl;
    cout << isotherms[1]->getInfoString() << endl;

    cout << setw(80) << setfill('=') << "    2. FIX_PY mode test    " << endl;
    Iast::Mode mode = Iast::Mode::FIX_PY;
    double pressure = 1.2345;
    vector<double> composition {0.56, 0.44};
    iast.setIsotherms(isotherms).calculate(mode, pressure, composition);

    double uptake = 0.0;
    vector<double> x;

    tie(uptake, x) = iast.getResult();

    cout << uptake << endl;
    for (auto a : x)
        cout << a << ", ";
    cout << endl;

    cout << setw(80) << setfill('=') << "    2-1. FIX_NX mode test    " << endl;
    mode = Iast::Mode::FIX_NX;
    tie(pressure, composition) = iast.calculate(mode, uptake, x).getResult();

    cout << pressure << endl;
    for (auto yy : composition)
        cout << yy << ", ";
    cout << endl;

    cout << setw(80) << setfill('=') << "    3. FIX_PX mode test    " << endl;
    vector<double> composition2 {1.0 / 3.0, 2.0 / 3.0};
    vector<double> y;
    tie (uptake,y) =  iast.calculate(Iast::Mode::FIX_PX, pressure, composition2).getResult();

    cout << uptake << endl;
    for (auto a : y)
        cout << a << ", ";
    cout << endl;

    cout << setw(80) << setfill('=') << "    4. IAST failure test    " << endl;
    // Operator Test.
    iast[1] = make_shared<LangmuirIsotherm>(10000000000000.0, 1.0);
    cout << iast[1]->getInfoString() << endl;

    try {
        iast.calculate(mode, pressure, composition);
        tie(uptake, x) = iast.getResult();

        cout << uptake << endl;
        for (auto a : x)
            cout << a << ", ";
        cout << endl;
        }
    catch (IastException& e)
        {
        cout << e.what() << endl;
        }

    cout << setw(80) << setfill('=') << "    5. Input exception test    " << endl;
    // Exception Test.
    try {
        vector<double> badComposition {1.0};
        iast.calculate(mode, pressure, badComposition);
        }
    catch (IastException& e)
        {
        cout << e.what() << endl;
        }

    try {
        Iast::IsothermVector badIsotherms {1, isotherms[0]};
        iast.setIsotherms(badIsotherms);
        iast.calculate(mode, pressure, composition);
        }
    catch (IastException& e)
        {
        cout << e.what() << endl;
        }

    iast.setIsotherms(isotherms);

    try {
        vector<double> badComposition {0.2, 0.3, 0.4};
        iast.calculate(mode, pressure, badComposition);
        }
    catch (IastException& e)
        {
        cout << e.what() << endl;
        }

    try {
        iast.calculate(10, pressure, composition);
        }
    catch (IastException& e)
        {
        cout << e.what() << endl;
        }

    return 0;
    }
catch (exception& e)
    {
    cout << e.what() << endl;
    }
