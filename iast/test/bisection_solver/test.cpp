#include <iostream>
#include <iomanip>
#include <memory>
#include <vector>

#include "../../solver.hpp"
#include "../../solver_factory.hpp"
#include "../../bisection_solver.hpp"

using namespace std;

double
func(const Solver::PointType& p)
    {
    double x = p[0];

    return -x * (x - 1.0);
    }

int
main(int argc, char* argv[])
    {
    SolverFactory factory;
    shared_ptr<Solver> sv = factory.create("bisection");

    vector<double> p = {0.5, 2.0};
    vector<Solver::FunctionType> fs = {func};

    sv->setFunctions(fs).
        setInitialPoint(p).
        setOption(BisectionSolver::Option::TOL, 1.0e-12);

    sv->solve();

    p = sv->getRootPoint();

    for (auto x : p)
        cout << x << ", ";
    cout << endl;

    for (auto f : fs)
        cout << f(p) << ", ";
    cout << endl;

    cout << sv->getNumFunctionCalls() << endl;

    p = {1.5, 2.0};

    try {
        sv->setInitialPoint(p).solve();
        }
    catch (SolverException& e)
        {
        cout << e.what() << endl;
        }

    try {
        sv->setOption(10, 1.0);
        }
    catch (SolverException& e)
        {
        cout << e.what() << endl;
        }

    fs.push_back(func);

    try {
        sv->setFunctions(fs);
        }
    catch (SolverException& e)
        {
        cout << e.what() << endl;
        }

    return 0;
    }
