#include <iostream>
#include <iomanip>
#include <memory>
#include "../../minimizer.hpp"
#include "../../simplex.hpp"
using namespace std;

double obj(const Minimizer::PointType& p)
    {
    double x = p[0];
    double y = p[1];
    //double z = p[2];

    //return x * x + (y - 1.0) * (y - 1.0) + z * (z - 1.0);
    return -(2.0 * x * y + 2.0 * x - x * x - 2.0 * y * y);
    }

int
main(int argc, char* argv[])
try {
    shared_ptr<Minimizer> mini = make_shared<Simplex>();
    vector<double> p = {-1.0, 1.0};

    mini->setFunction(obj).
        setInitialPoint(p).
        setOption(Simplex::Option::NUM_REPEATS, 5).
        setOption(Simplex::Option::TOL_X, 1e-8).
        setOption(Simplex::Option::TOL_F, 1e-8);

    mini->minimize();

    p = mini->getMinimumPoint();

    for (auto x : p)
        cout << x << ", ";
    cout << endl;

    cout << mini->getMinimumValue() << endl;
    cout << mini->getNumFunctionCalls() << endl;

    return 0;
    }
catch(exception& e)
    {
    cout << e.what() << endl;
    }
