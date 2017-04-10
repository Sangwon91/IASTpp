#pragma once

#include "isotherm.hpp"
#include "langmuir_isotherm.hpp"

class DslIsotherm : public Isotherm
    {
public:
    DslIsotherm(double q1, double k1, double q2, double k2);
    virtual ~DslIsotherm() = default;

    virtual double loading(double P) const override;
    virtual double spressure(double P) const override;

    virtual std::string getInfoString() const override;
    virtual ParameterType getParameters() const override;
private:
    LangmuirIsotherm iso1;
    LangmuirIsotherm iso2;
    double params[4];
    };
