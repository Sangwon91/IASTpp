#pragma once

#include "solver.hpp"
#include <vector>

#include "Eigen/Dense"

class BacktrackingSolver : public Solver
    {
public:
             BacktrackingSolver();
    virtual ~BacktrackingSolver() = default;

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
    int mNumFunctionCalls;

    // Notation from Numerical recipes
    void approxJacobiMatrix(const Eigen::VectorXd& x,
                                  Eigen::MatrixXd& jacobi);

    void evaluateFunctionVector(const Eigen::VectorXd& x,
                                      Eigen::VectorXd& fs);

    double evaluateObjective(const Eigen::VectorXd& x);

    void updatePoint(const Eigen::VectorXd& df,
                     const Eigen::VectorXd& p,
                           Eigen::VectorXd& x);
    };
