#pragma once

#include "interpolator.hpp"

class LinearInterpolator : public Interpolator
    {
public:
             LinearInterpolator() = default;
    virtual ~LinearInterpolator() = default;

    virtual std::vector<double>& getXData() override;
    virtual std::vector<double>& getYData() override;
    virtual double at(double x) const override;
    virtual double operator () (double x) const override;

    virtual Interpolator& setData(const std::vector<double>& x,
                                  const std::vector<double>& y) override;
    virtual Interpolator& pushBack(double x, double y) override;

    virtual Interpolator& setOption(int option, double value) override;
private:
    std::vector<double> xs;
    std::vector<double> ys;
    };
