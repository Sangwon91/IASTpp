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

#include "simplex.hpp"
#include "minimizer.hpp"

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

        double pDes = 1.0;
        vector<double> yIn {0.70, 0.15, 0.005, 0.145};

        for (double pAds = 5.0; pAds <= 25.0; pAds += 0.5)
            {
            double totalUptakeIn;
            vector<double> xIn;

            tie(totalUptakeIn, xIn) = iast.
                calculate(Iast::Mode::FIX_PY, pAds, yIn).
                getResult();

            vector<double> uptakesIn (4);

            for (int i = 0; i < 4; ++i)
                uptakesIn[i] = totalUptakeIn * xIn[i];

            function<double(const Minimizer::PointType&)> obj =
                [&iast, &uptakesIn, &pDes, &yIn](const Minimizer::PointType& y)
                    {
                    double totalUptakeOut;
                    vector<double> xOut (4);
                    vector<double> yOut (y);
                    vector<double> yCaled (4);

                    for (const auto& yi : y)
                        if (yi < 0.0 or yi > 1.0)
                            return 1.0e30;

                    try {
                        tie(totalUptakeOut, xOut) = iast.
                            calculate(Iast::Mode::FIX_PY, pDes, yOut).
                            getResult();
                        }
                    catch (IastException& e)
                        {
                        return 1.0e30;
                        }

                    for (int i = 0; i < 4; ++i)
                        yCaled[i] = uptakesIn[i] - totalUptakeOut * xOut[i];

                    ::normalize(yCaled);

                    double error = 0.0;
                    for (int i = 0; i < 4; ++i)
                        {
                        double err = yCaled[i] - yOut[i];
                        error += err * err;
                        }

                    return error / 4.0;
                    };

            shared_ptr<Minimizer> sp = make_shared<Simplex>();
            vector<double> yOut {yIn};

            sp->setFunction(obj).
                setInitialPoint(yOut).
                setOption(Simplex::Option::TOL_X, 1.e-3).
                setOption(Simplex::Option::TOL_F, 1.e-6).
                minimize();

            yOut = sp->getMinimumPoint();

            for (auto& y : yOut)
                cout << y << "    ";
            cout << sp->getNumFunctionCalls();
            //for (auto& y : yCaled)
            //    cout << y << "    ";
            cout << endl;
            }
        }
    catch (IastException& e)
        {
        cout << e.what() << endl;
        }

    return 0;
    }
