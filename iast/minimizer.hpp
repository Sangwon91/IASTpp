#pragma once

#include <vector>
#include <functional>

#include "minimizer_exception.hpp"

class Minimizer
    {
public:
    using ValueType    = double;
    using PointType    = std::vector<ValueType>;
    using FunctionType = std::function< double(const PointType&) >;

             Minimizer() = default;
    virtual ~Minimizer() = default;

    virtual Minimizer& setFunction(FunctionType func) = 0;
    virtual Minimizer& setInitialPoint(PointType point) = 0;
    virtual Minimizer& setOption(int option, ValueType value) = 0;

    virtual Minimizer& minimize() = 0;

    virtual PointType  getMinimumPoint() const = 0;
    virtual ValueType  getMinimumValue() const = 0;
    virtual int        getNumFunctionCalls() const = 0;
    };
