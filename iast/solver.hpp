#pragma once

#include <vector>
#include <functional>

#include "solver_exception.hpp"

class Solver
    {
public:
    using ValueType = double;
    using PointType = std::vector<double>;
    using FunctionType = std::function< double(const PointType&) >;

             Solver() = default;
    virtual ~Solver() = default;

    virtual Solver& setFunctions(std::vector<FunctionType> functions) = 0;
    virtual Solver& setInitialPoint(PointType point) = 0;
    virtual Solver& setOption(int option, ValueType value) = 0;

    virtual Solver& solve() = 0;

    virtual PointType getRootPoint() const = 0;
    virtual int getNumFunctionCalls() const = 0;
    };
