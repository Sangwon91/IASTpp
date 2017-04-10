#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <tuple>
#include <functional>
#include <cmath>

#include "iast.hpp"
//#include "../../isotherm_factory.hpp"
//#include "../../interpolator_isotherm.hpp"

#include "isotherm_utility.hpp"
#include "isotherm_factory.hpp"

#include "solver.hpp"
#include "simplex_solver.hpp"
#include "solver_factory.hpp"

using namespace std;

void
normalize(vector<double>& v)
    {
    double sum = 0.0;
    for (const auto& vi : v)
        sum += vi;

    for (auto& vi : v)
        vi /= sum;
    }

int
main(int argc, char* argv[])
    {
    try {
        vector<double> x, y;

        IsothermModeler modeler;
        Iast::IsothermVector isotherms;
        isotherms.resize(4);

        IsothermFactory factory;
        isotherms[0] = factory.create("n2.iso");
        isotherms[1] = factory.create("co2.iso");
        ::readTwoColumns("h2o.dat", x, y);
        isotherms[2] = factory.create("interpolator", {x, y});
        isotherms[3] = factory.create("o2.iso");

        for (const auto& iso : isotherms)
            cout << iso->getInfoString() << endl;

        Iast iast;
        iast.setIsotherms(isotherms);

        double pDes = 1.0;
        vector<double> yIn {0.70, 0.15, 0.005, 0.145};

        double nn;
        vector<double> xx;
        tie(nn, xx) = iast.calculate(Iast::Mode::FIX_PY, 5.0, yIn).getResult();

        for (const auto& x : xx)
            cout << x * nn << "    ";
        cout << endl;

        for (double pAds = 5.0; pAds <= 20.0; pAds += 1.0)
            {
            double totalUptakeIn;
            vector<double> xIn;

            tie(totalUptakeIn, xIn) = iast.
                calculate(Iast::Mode::FIX_PY, pAds, yIn).
                getResult();

            vector<double> uptakesIn (4);

            for (int i = 0; i < 4; ++i)
                uptakesIn[i] = totalUptakeIn * xIn[i];

            cout << pAds << "    ";

            for (int i = 0; i< 4; ++i)
                cout << uptakesIn[i] << "    ";

            double totalUptakeOut;
            vector<double> xOut (4);
            vector<double> yOut (yIn);
            vector<double> yCaled (4);

            for (int iter = 0; iter < 1000; ++iter)
                {
                tie(totalUptakeOut, xOut) = iast.
                    calculate(Iast::Mode::FIX_PY, pDes, yOut).
                    getResult();

                for (int i = 0; i < 4; ++i)
                    yCaled[i] = uptakesIn[i] - totalUptakeOut * xOut[i];

                ::normalize(yCaled);
                //cout << iter << "    ";
                //for (auto& y : yCaled)
                //     cout << y << "    ";
                //cout << endl;

                double error = 0.0;
                for (int i = 0; i < 4; ++i)
                    {
                    double err = yCaled[i] - yOut[i];
                    error += err * err;
                    }

                if (error < 1.0-10)
                    break;

                double lam = 0.999;
                for (int i = 0; i < 4; ++i)
                    yOut[i] = lam * yOut[i] + (1.0 - lam) * yCaled[i];
                }

            for (auto& y : yOut)
                cout << y << "    ";
            for (auto& y : yCaled)
                cout << y << "    ";
            cout << endl;
            }
        }
    catch (IastException& e)
        {
        cout << e.what() << endl;
        }

    return 0;
    }
