#include "simplex_solver.hpp"

#include <iostream>
#include <cmath>
#include <ciso646>

SimplexSolver::SimplexSolver() :
    mFunctions {},
    mInitialPoint {},
    mRootPoint {},
    mNumRepeats {5},
    mTolX {1.0e-10},
    mTolF {1.0e-10},
    mNumFunctionCalls {}
    {

    }

Solver&
SimplexSolver::setFunctions(std::vector<FunctionType> functions)
    {
    mFunctions = functions;
    return *this;
    }

Solver&
SimplexSolver::setInitialPoint(PointType point)
    {
    mInitialPoint = point;
    return *this;
    }

Solver&
SimplexSolver::setOption(int option, ValueType value)
    {
    switch (option)
        {
        case Option::NUM_REPEATS:
            mNumRepeats = static_cast<int>(value + 1.0e-5);
            break;
        case Option::TOL_X:
            mTolX = value;
            break;
        case Option::TOL_F:
            mTolF = value;
            break;
        default:
            throw SolverException {__FILE__, __LINE__, "Unsupported option."};
            break;
        }

    return *this;
    }

Solver&
SimplexSolver::solve()
    {
    if (mFunctions.size() != mInitialPoint.size())
        throw SolverException {__FILE__, __LINE__, "# of functions != # of dimension."};

    FunctionType objective = [this](const PointType& p)
        {
        double value = 0.0;
        for (auto& func : mFunctions)
            {
            ValueType f = func(p);
            value += f * f;
            }

        return value;
        };

    Simplex simplex;

    simplex.setFunction(objective).
        setInitialPoint(mInitialPoint).
        setOption(Simplex::Option::NUM_REPEATS, mNumRepeats).
        setOption(Simplex::Option::TOL_X, mTolX).
        setOption(Simplex::Option::TOL_F, mTolF);

    simplex.minimize();

    auto minValue = simplex.getMinimumValue();
    if (minValue > mTolF * 100.0 or !std::isnormal(minValue))
        throw NoRootException {__FILE__, __LINE__, "No root found."};

    mRootPoint = simplex.getMinimumPoint();
    mNumFunctionCalls = simplex.getNumFunctionCalls();

    return *this;
    }

Solver::PointType
SimplexSolver::getRootPoint() const
    {
    return mRootPoint;
    }

int
SimplexSolver::getNumFunctionCalls() const
    {
    return mNumFunctionCalls * mFunctions.size();
    }
