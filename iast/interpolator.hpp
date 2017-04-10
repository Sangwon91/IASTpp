#pragma once

#include <utility>
#include <vector>

class Interpolator
    {
public:
             Interpolator() = default;
    virtual ~Interpolator() = default;

    virtual std::vector<double>& getXData() = 0;
    virtual std::vector<double>& getYData() = 0;

    virtual double at(double x) const = 0;
    // Operators.
    virtual double operator () (double x) const = 0;

    // Data setting.
    virtual Interpolator& setData(const std::vector<double>& x,
                                  const std::vector<double>& y) = 0;
    virtual Interpolator& pushBack(double x, double y) = 0;

    // Option
    virtual Interpolator& setOption(int option, double value) = 0;
    };
