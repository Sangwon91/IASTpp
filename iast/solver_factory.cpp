#include "solver_factory.hpp"

#include <ciso646>

#include "simplex_solver.hpp"
#include "bisection_solver.hpp"
//#include "arma_solver.hpp"

std::shared_ptr<Solver>
SolverFactory::create(std::string name)
    {
    if (name == "simplex")
        return std::make_shared<SimplexSolver>();
//    else if (name == "arma")
//        return std::make_shared<ArmaSolver>();
    else if (name == "bisection")
        return std::make_shared<BisectionSolver>();
    else
        throw SolverException {__FILE__, __LINE__, "Unsupported solver."};
    }
