#pragma once

#include "solver.hpp"

class BisectionSolver : public Solver
    {
public:
    enum Option : int {TOL};

             BisectionSolver();
    virtual ~BisectionSolver() = default;

    virtual Solver& setFunctions(std::vector<FunctionType> functions) override;
    virtual Solver& setInitialPoint(PointType point) override;
    virtual Solver& setOption(int option, ValueType value) override;

    virtual Solver& solve() override;

    virtual PointType getRootPoint() const override;
    virtual int getNumFunctionCalls() const override;
private:
    ValueType mHigh;
    ValueType mLow;
    ValueType mTol;
    PointType mRootPoint;
    int mNumFunctionCalls;
    std::function<double(double)> mFunction;
    };
