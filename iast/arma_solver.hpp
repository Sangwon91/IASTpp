#pragma once
#include "solver.hpp"

class ArmaSolver : public Solver
    {
public:
             ArmaSolver();
    virtual ~ArmaSolver() = default;

    virtual Solver& setFunctions(std::vector<FunctionType> functions) override;
    virtual Solver& setInitialPoint(PointType point) override;
    virtual Solver& setOption(int option, ValueType value) override;

    virtual Solver& solve() override;

    virtual PointType getRootPoint() const override;
    virtual int getNumFunctionCalls() const override;
private:
    PointType mRootPoint;
    PointType mInitialPoint;
    int mNumFunctionCalls;
    std::vector<FunctionType> mFunctions;
    };
