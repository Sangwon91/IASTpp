#include <iostream>
#include <iomanip>
#include <memory>
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
    std::shared_ptr<Isotherm> iso = factory.create("langmuir", {1.0, 1.0});

    double p  = 5.5;
    double dp = 1.e-6;

    cout << "1." << endl;
    cout << iso->getInfoString() << endl;
    cout << iso->loading(p) << " = ";
    cout << (iso->spressure(p + dp) - iso->spressure(p)) / dp * p << endl;

    iso = factory.create("lf", {1.0, 1.0, 2.0});

    cout << "2." << endl;
    cout << iso->getInfoString() << endl;
    cout << iso->loading(p) << " = ";
    cout << (iso->spressure(p + dp) - iso->spressure(p)) / dp * p << endl;

    iso = factory.create("dsl", {1.0, 1.0, 2.0, 2.0});

    cout << "3." << endl;
    cout << iso->getInfoString() << endl;
    cout << iso->loading(p) << " = ";
    cout << (iso->spressure(p + dp) - iso->spressure(p)) / dp * p << endl;

    iso = factory.create("dslf", {1.0, 1.0, 1.0, 2.0, 2.0, 2.0});

    cout << "4." << endl;
    cout << iso->getInfoString() << endl;
    cout << iso->loading(p) << " = ";
    cout << (iso->spressure(p + dp) - iso->spressure(p)) / dp * p << endl;

    vector<double> x {1.0, 2.0, 3.0, 4.0, 5.0};
    vector<double> y {0.5, 0.6666, 0.75, 0.8, 0.8333333};

    iso = factory.create("interpolator", {x, y});

    dynamic_pointer_cast<InterpolatorIsotherm>(iso)->pushBack(6.0, 0.857143);

    cout << "5." << endl;
    cout << iso->getInfoString() << endl;
    cout << iso->loading(p) << " = ";
    cout << (iso->spressure(p + dp) - iso->spressure(p)) / dp * p << endl;

    try {
        factory.create("what?", {0});
        }
    catch(IsothermException& e)
        {
        cout << e.what() << endl;
        }

    try {
        factory.create("lf", {1, 1, 3});
        }
    catch(IsothermException& e)
        {
        cout << e.what() << endl;
        }

    // Utility test.

    double uptake = iso->loading(p);
    cout << "Uptake = " << uptake << " at pressure = " << inverseIsotherm(*iso, uptake) << endl;

    try {
        inverseIsotherm(*iso, 100000.0);
        }
    catch (IsothermException& e)
        {
        cout << e.what() << endl;
        }

    // Item functionality.
    {
        shared_ptr<Isotherm> refiso = factory.create("langmuir", {1.0, 1.0});
        function<double(double)> isoheat = [](double p){return 40.0;};
        iso = factory.create("item", {refiso, isoheat, 298.0, 350.0});

        cout << "6." << endl;
        cout << iso->getInfoString() << endl;
        cout << iso->loading(p) << " = ";
        cout << (iso->spressure(p + dp) - iso->spressure(p)) / dp * p << endl;
    }

    iso = factory.create("bet", {1.0, 2.0, 3.0});

    cout << "7." << endl;
    cout << iso->getInfoString() << endl;
    cout << iso->loading(p) << " = ";
    cout << (iso->spressure(p + dp) - iso->spressure(p)) / dp * p << endl;

    iso = factory.create("henry", {2.0});

    cout << "8." << endl;
    cout << iso->getInfoString() << endl;
    cout << iso->loading(p) << " = ";
    cout << (iso->spressure(p + dp) - iso->spressure(p)) / dp * p << endl;

    iso = factory.create("quadratic", {1.0, 2.0, 3.0});

    cout << "9." << endl;
    cout << iso->getInfoString() << endl;
    cout << iso->loading(p) << " = ";
    cout << (iso->spressure(p + dp) - iso->spressure(p)) / dp * p << endl;

    for (auto& m : iso->getParameters())
        cout << m.first << "   " << m.second << endl;

    return 0;
    }
catch (exception& e)
    {
    cout << e.what() << endl;
    }
