#include <iostream>
#include <iomanip>
#include <memory>
#include <vector>

#include "../../solver.hpp"
#include "../../simplex_solver.hpp"
#include "../../solver_factory.hpp"

using namespace std;

double
func1(const Simplex::PointType& p)
    {
    double x1 = p[0];
    double x2 = p[1];
    double x3 = p[2];

    return x1 * x1 - cos(x1 * x2 * x3);
    }

double
func2(const Simplex::PointType& p)
    {
    double x1 = p[0];
    double x2 = p[1];
    double x3 = p[2];

    return exp(x1 * x3 * x3) + x2;
    }

double
func3(const Simplex::PointType& p)
    {
    double x1 = p[0];
    double x2 = p[1];
    double x3 = p[2];

    return pow(x1 + x2 + x3, 2);
    }

int
main(int argc, char* argv[])
    {
    SolverFactory factory;
    shared_ptr<Solver> sv = factory.create("simplex");

    vector<double> p = {5.0, -2.0, 2.0};
    vector<Solver::FunctionType> fs= {func1, func2, func3};

    sv->setFunctions(fs).
        setInitialPoint(p).
        setOption(SimplexSolver::Option::NUM_REPEATS, 2).
        setOption(SimplexSolver::Option::TOL_X, 1e-8).
        setOption(SimplexSolver::Option::TOL_F, 1e-8);

    sv->solve();

    p = sv->getRootPoint();

    for (auto x : p)
        cout << x << ", ";
    cout << endl;

    for (auto f : fs)
        cout << f(p) << ", ";
    cout << endl;

    cout << sv->getNumFunctionCalls() << endl;

    p = {1.0, 2.0};

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

    fs[2] = [](const SimplexSolver::PointType& p) {return 1.0;};

    try {
        p = {1.0, 2.0, 3.0};

        sv->setFunctions(fs).setInitialPoint(p).solve();
        }
    catch (NoRootException& e)
        {
        cout << "File: " << __FILE__ << ", Line: " << __LINE__ << ": ";
        cout << e.what() << endl;
        }
    catch (SolverException& e)
        {
        cout << e.what() << endl;
        }

    return 0;
    }
