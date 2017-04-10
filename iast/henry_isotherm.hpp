#pragma once

#include "isotherm.hpp"

class HenryIsotherm : public Isotherm
    {
public:
    HenryIsotherm(double k);
    virtual ~HenryIsotherm() = default;

    virtual double loading(double P) const override;
    virtual double spressure(double P) const override;

    virtual std::string getInfoString() const override;
    virtual ParameterType getParameters() const override;
private:
    double mK;
    };
