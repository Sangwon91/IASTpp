#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <tuple>

#include "../../iast.hpp"
//#include "../../isotherm_factory.hpp"
//#include "../../interpolator_isotherm.hpp"

#include "../../isotherm_utility.hpp"

using namespace std;

int
main(int argc, char* argv[])
    {
    try {
        IsothermModeler modeler;
        Iast::IsothermVector isotherms;
        isotherms.resize(4);

        vector<double> x, y;
        ::readTwoColumns("n2.dat", x, y);
        isotherms[0] = modeler.autofit(x, y); // n2
        ::readTwoColumns("co2.dat", x, y);
        isotherms[1] = modeler.autofit(x, y); // co2
        ::readTwoColumns("h2o.dat", x, y);
        isotherms[2] = modeler.autofit(x, y); // h2o
        ::readTwoColumns("o2.dat", x, y);
        isotherms[3] = modeler.autofit(x, y); // o2

        for (const auto& iso : isotherms)
            cout << iso->getInfoString() << endl;

        Iast iast;
        iast.setIsotherms(isotherms);

        vector<double> gasComposition {0.840, 0.140, 0.015, 0.005};

        constexpr double tiny = 1.0e-10;
        for (double p = 0.1; p < 20.0 + tiny; p += 0.1)
            {
            double uptake;
            vector<double> composition;

            try {
                iast.calculate(Iast::Mode::FIX_PY, p, gasComposition);
                tie(uptake, composition) = iast.getResult();
                }
            catch (IastException& e)
                {
                cerr << e.what() << endl;
                continue;
                }

            cout << p << "    ";
            cout << uptake << "    ";

            for (auto& comp : composition)
                cout << comp << "    ";

            for (int i = 0; i < 4; ++i)
                {
                double pp = gasComposition[i] / composition[i] * p;

                cout << isotherms[i]->spressure(pp) << "    ";
                }
            cout << endl;
            }
        }
    catch (IastException& e)
        {
        cout << e.what() << endl;
        }

    return 0;
    }
