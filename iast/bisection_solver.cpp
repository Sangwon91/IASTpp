#include "bisection_solver.hpp"

#include <cmath>
#include <ciso646>

BisectionSolver::BisectionSolver() :
    mHigh {},
    mLow {},
    mTol {1.e-8},
    mRootPoint {},
    mNumFunctionCalls {},
    mFunction {}
    {

    }

Solver&
BisectionSolver::setFunctions(std::vector<FunctionType> functions)
    {
    if (functions.size() != 1)
        throw SolverException {__FILE__, __LINE__, "1D only."};

    mFunction = [this, functions](const ValueType& p)
        {
        mNumFunctionCalls++;
        return functions[0]({p});
        };

    return *this;
    }

Solver&
BisectionSolver::setInitialPoint(PointType point)
    {
    mLow  = point[0];
    mHigh = point[1];

    return *this;
    }

Solver&
BisectionSolver::setOption(int option, ValueType value)
    {
    switch(option)
        {
        case Option::TOL:
            mTol = value;
            break;
        default:
            throw SolverException {__FILE__, __LINE__, "Unsupported option."};
            break;
        }

    return *this;
    }

Solver&
BisectionSolver::solve()
    {
    mNumFunctionCalls = 0;
    // Simple Bisection Algorithm
    double xLow    = mLow;
    double xHigh   = mHigh;
    double xCenter = 0.0;

    double fLow    = mFunction(xLow);
    double fHigh   = mFunction(xHigh);
    double fCenter = 0.0;

    if (fLow * fHigh > 0.0)
        throw NoRootException {__FILE__, __LINE__, "f(low) * f(high) > 0."};

    int maxIter = static_cast<int>(std::log2((xHigh - xLow)/ mTol)) + 1;

    for (int iter = 0; iter < maxIter; ++iter)
        {
        xCenter = 0.5 * (xLow + xHigh);
        fCenter = mFunction(xCenter);

        if (fCenter * fHigh > 0.0)
            {
            xHigh = xCenter;
            fHigh = fCenter;
            }
        else
            {
            xLow = xCenter;
            fLow = fCenter;
            }
        }

    mRootPoint = {xCenter};

    return *this;
    }

Solver::PointType
BisectionSolver::getRootPoint() const
    {
    return mRootPoint;
    }

int
BisectionSolver::getNumFunctionCalls() const
    {
    return mNumFunctionCalls;
    }
