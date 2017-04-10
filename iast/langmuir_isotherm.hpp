#pragma once

#include "isotherm.hpp"

class LangmuirIsotherm : public Isotherm
    {
public:
    LangmuirIsotherm(double q, double k);
    virtual ~LangmuirIsotherm() = default;

    virtual double loading(double P) const override;
    virtual double spressure(double P) const override;

    virtual std::string getInfoString() const override;
    virtual ParameterType getParameters() const override;
private:
    double mQ;
    double mK;
    };
