#pragma once

#include <memory>
#include <string>

#include "isotherm.hpp"
#include "interpolator.hpp"

class InterpolatorIsotherm : public Isotherm
    {
public:
    InterpolatorIsotherm(const std::vector<double>& x,
                         const std::vector<double>& y);

    virtual ~InterpolatorIsotherm() = default;

    virtual double loading(double P) const override;
    virtual double spressure(double P) const override;

    virtual std::string getInfoString() const override;
    virtual ParameterType getParameters() const override;

    Interpolator& getInterpolator();
    void pushBack(double p, double n);
private:
    mutable std::shared_ptr<Interpolator> mLoading;
    std::vector<double> mSpressure;
    };
