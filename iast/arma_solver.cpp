#include "arma_solver.hpp"

#include "root_finder.hpp"

ArmaSolver::ArmaSolver() :
    mRootPoint {},
    mInitialPoint {},
    mNumFunctionCalls {},
    mFunctions {}
    {

    }

Solver&
ArmaSolver::setFunctions(std::vector<FunctionType> functions)
    {
    mFunctions = functions;
    return *this;
    }

Solver&
ArmaSolver::setInitialPoint(PointType point)
    {
    mInitialPoint = point;
    return *this;
    }

Solver&
ArmaSolver::setOption(int option, ValueType value)
    {
    throw SolverException {__FILE__, __LINE__, "Unsupported option"};

    return *this;
    }

Solver&
ArmaSolver::solve()
    {
    if (mFunctions.size() != mInitialPoint.size())
        throw SolverException {__FILE__, __LINE__, "# of functions != dimension."};

    int dim = mFunctions.size();

    root_finder rf;

    root_finder::vec init;
    init.resize(dim);

    for (int i = 0; i < dim; ++i)
        init[i] = mInitialPoint[i];

    rf.set_initial_guess(init);

    mNumFunctionCalls = 0;
    for (int i = 0; i < dim; ++i)
        {
        rf.add_eqn([this, i, dim](const root_finder::vec& v)
            {
            PointType p;
            p.resize(dim);

            for (int j = 0; j < dim; ++j)
                p[j] = v[j];

            mNumFunctionCalls++;

            return mFunctions[i](p);
            });
        }

    root_finder::vec root;
    try {
        root = rf.solve();
        }
    catch (std::exception& e)
        {
        throw NoRootException {__FILE__, __LINE__, "No root found."};
        }

    mRootPoint.resize(dim);
    for (int i = 0; i < dim; ++i)
        mRootPoint[i] = root[i];

    //mNumFunctionCalls = rf.get_iterations();

    return *this;
    }

Solver::PointType
ArmaSolver::getRootPoint() const
    {
    return mRootPoint;
    }

int
ArmaSolver::getNumFunctionCalls() const
    {
    return mNumFunctionCalls;
    }
