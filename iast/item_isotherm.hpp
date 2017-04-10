#pragma once

#include "isotherm.hpp"
#include "interpolator_isotherm.hpp"
#include <memory>
#include <vector>
#include <functional>

class ItemIsotherm : public Isotherm
    {
public:
    using IsothermPtr = std::shared_ptr<Isotherm>;
    using FunctorType = std::function<double(double)>;

    ItemIsotherm(IsothermPtr isotherm,
                 FunctorType isoheat, // isosteric head of adsorption at refTemper
                 double refTemperature,
                 double tarTemperature);

    virtual ~ItemIsotherm();

    virtual double loading(double p) const override;
    virtual double spressure(double p) const override;

    virtual std::string getInfoString() const override;
    virtual ParameterType getParameters() const override;
private:
    IsothermPtr mRefIsotherm;
    FunctorType mIsoheat;
    double mRefTemperature;
    double mTarTemperature;

    mutable InterpolatorIsotherm mIsotherm;

    void expand(double p) const;
    double newPressure(double n) const;
    };
