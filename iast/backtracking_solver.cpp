#include "backtracking_solver.hpp"
#include <ciso646>

BacktrackingSolver::BacktrackingSolver()
    {

    }

Solver&
BacktrackingSolver::setFunctions(std::vector<FunctionType> functions)
    {
    mFunctions = functions;

    return *this;
    }

Solver&
BacktrackingSolver::setInitialPoint(PointType point)
    {
    mInitialPoint = point;

    return *this;
    }

Solver&
BacktrackingSolver::setOption(int option, ValueType value)
    {

    return *this;
    }

Solver&
BacktrackingSolver::solve()
    {
    int size = mInitialPoint.size();

    Eigen::VectorXd x (size);
    Eigen::MatrixXd jacobi (size, size);
    Eigen::VectorXd fs (size);

    for (int i = 0; i < size; ++i)
        x(i) = mInitialPoint[i];

    for (int iter = 0; iter < 100; ++iter)
        {
        evaluateFunctionVector(x, fs);

        if (fs.norm() / size < 1.0e-10)
            break;

        approxJacobiMatrix(x,  jacobi);

        Eigen::ColPivHouseholderQR<Eigen::MatrixXd> linSolver (jacobi);
        Eigen::VectorXd p = -linSolver.solve(fs);

        Eigen::VectorXd df = jacobi.transpose() * fs;

        updatePoint(df, p, x);
        }

    mRootPoint.resize(size);
    for (int i = 0; i < size; ++i)
        mRootPoint[i] = x(i);

    return *this;
    }

Solver::PointType
BacktrackingSolver::getRootPoint() const
    {
    return mRootPoint;
    }

int
BacktrackingSolver::getNumFunctionCalls() const
    {
    return mNumFunctionCalls;
    }

void
BacktrackingSolver::approxJacobiMatrix(const Eigen::VectorXd& x_,
                                             Eigen::MatrixXd& jacobi)
    {
    constexpr double dx = 1.0e-6;

    auto& funcs = mFunctions;

    int dim = funcs.size();

    std::vector<double> x (dim);
    for (int i = 0; i < dim; ++i)
        x[i] = x_(i);

    std::vector<double> xx (x);

    for (int i = 0; i < dim; ++i)
        {
        double funcix = funcs[i](x);
        for (int j = 0; j < dim; ++j)
            {
            xx[j] += dx;

            jacobi(i, j) = (funcs[i](xx) - funcix) / dx;

            xx[j] -= dx;
            }
        }
    }

void
BacktrackingSolver::evaluateFunctionVector(const Eigen::VectorXd& x_,
                                                 Eigen::VectorXd& fs)
    {
    auto& funcs = mFunctions;
    int size = x_.size();

    std::vector<double> x (size);
    for (int i = 0; i < size; ++i)
        x[i] = x_(i);

    for (int i = 0; i < size; ++i)
        fs(i) = funcs[i](x);
    }

double
BacktrackingSolver::evaluateObjective(const Eigen::VectorXd& x_)
    {
    auto& funcs = mFunctions;

    int size = x_.size();

    std::vector<double> x (size);
    for (int i = 0; i < size; ++i)
        x[i] = x_(i);

    double obj = 0.0;
    for (int i = 0; i < size; ++i)
        {
        double fi = funcs[i](x);
        obj += fi * fi;
        }

    obj *= 0.5;

    return obj;
    }

void
BacktrackingSolver::updatePoint(const Eigen::VectorXd& df,
                                const Eigen::VectorXd& p,
                                      Eigen::VectorXd& x)
    {
    constexpr double alpha = 1.0e-4;

    Eigen::VectorXd xnew = x + p;

    double g1 = evaluateObjective(xnew);
    double g0 = evaluateObjective(x);
    double dg0 = df.dot(p);

    double lam = 1.0;

    int maxIter = 10;
    // Just do full Newton step.
    if (g1 < g0 + alpha * lam * dg0)
        maxIter = 0;

    double lamOld = lam;
    for (int iter = 0; iter < maxIter; ++iter)
        {
        // Quadratic function assumption.
        lam = - 0.5 * dg0 / (g1 - g0 - dg0);

        // Avoid too small or too big changes.
        if (lam < 0.1 * lamOld)
            lam = 0.1 * lamOld;
        else if (lam > 0.5 * lamOld)
            lam = 0.5 * lamOld;

        // Try new point.
        xnew = x + lam * p;
        g1 = evaluateObjective(xnew);

        if (g1 < g0 + alpha * lam * dg0)
            break;

        lamOld = lam;
        }

    // Program must have to reach here.
    x = xnew;
    }
