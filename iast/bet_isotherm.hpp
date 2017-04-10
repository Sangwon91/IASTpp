#pragma once

#include "isotherm.hpp"

class BetIsotherm : public Isotherm
    {
public:
             BetIsotherm(double _q, double _k1, double _k2);
    virtual ~BetIsotherm();

    virtual double loading(double p) const override;
    virtual double spressure(double p) const override;

    virtual std::string getInfoString() const override;
    virtual ParameterType getParameters() const override;
private:
    double q;
    double k1;
    double k2;
    };
