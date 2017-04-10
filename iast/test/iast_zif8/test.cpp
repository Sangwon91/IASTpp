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
        //isotherms[0] = modeler.autofit(x, y); // n2
        isotherms[0] = modeler.fit("langmuir", x, y); // n2
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

        vector<double> gasComposition {0.77, 0.15, 0.045, 0.035};

        constexpr double tiny = 1.0e-10;
        constexpr double dp = 0.01;
        for (double p = dp; p < 1.0 + tiny; p += dp)
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
