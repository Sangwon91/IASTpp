#pragma once

#include "isotherm.hpp"

class LfIsotherm : public Isotherm
    {
public:
    LfIsotherm(double q, double k, double n);
    virtual ~LfIsotherm() = default;

    virtual double loading(double P) const override;
    virtual double spressure(double P) const override;

    virtual std::string getInfoString() const override;
    virtual ParameterType getParameters() const override;
private:
    double mQ;
    double mK;
    double mN;
    };
