#include <iostream>
#include <functional>
#include <vector>
#include <cstdlib>

#include "../../isotherm_factory.hpp"
#include "../../isotherm_utility.hpp"
#include "../../linear_interpolator.hpp"

using namespace std;

int
main(int argc, char* argv[])
    {
    vector<double> x;
    vector<double> y;

    // Read Q and make data.
    ::readTwoColumns("Q.dat", x, y);
    LinearInterpolator q;
    q.setData(x, y);

    function<double(double)> isoheat = q;

    IsothermFactory factory;
    IsothermModeler modeler;

    // Read n and make isotherm.
    ::readTwoColumns("n.dat", x, y);
    auto n = modeler.autofit(x, y);

    double tar = std::atof(argv[1]);

    // apply item.
    auto iso = factory.create("item", {n, isoheat, 293.0, tar});

    double dp = 0.1;
    for (double p = 0.0; p <= 10.0; p += dp)
        cout << p << "  " << iso->loading(p) << endl;

    return 0;
    }
