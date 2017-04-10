#pragma once

#include "solver.hpp"
#include "simplex.hpp"
#include <vector>

class SimplexSolver : public Solver
    {
public:
    enum Option : int {NUM_REPEATS, TOL_X, TOL_F};

             SimplexSolver();
    virtual ~SimplexSolver() = default;

    virtual Solver& setFunctions(std::vector<FunctionType> functions) override;
    virtual Solver& setInitialPoint(PointType point) override;
    virtual Solver& setOption(int option, ValueType value) override;

    virtual Solver& solve() override;

    virtual PointType getRootPoint() const override;
    virtual int       getNumFunctionCalls() const override;
private:
    std::vector<FunctionType> mFunctions;
    PointType mInitialPoint;
    PointType mRootPoint;
    int mNumRepeats;
    ValueType mTolX;
    ValueType mTolF;
    int mNumFunctionCalls;
    };
