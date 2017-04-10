#pragma once

#include "isotherm.hpp"
#include "lf_isotherm.hpp"

class DslfIsotherm : public Isotherm
    {
public:
    DslfIsotherm(double q1, double k1, double n1,
                 double q2, double k2, double n2);

    virtual ~DslfIsotherm() = default;

    virtual double loading(double P) const override;
    virtual double spressure(double P) const override;

    virtual std::string getInfoString() const override;
    virtual ParameterType getParameters() const override;
private:
    LfIsotherm iso1;
    LfIsotherm iso2;
    double params[6];
    };
