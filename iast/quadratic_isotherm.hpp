#pragma once

#include "isotherm.hpp"

class QuadraticIsotherm : public Isotherm
    {
public:
             QuadraticIsotherm(double _q, double _k1, double _k2);
    virtual ~QuadraticIsotherm();

    virtual double loading(double p) const override;
    virtual double spressure(double p) const override;

    virtual std::string getInfoString() const override;
    virtual ParameterType getParameters() const override;
private:
    double q;
    double k1;
    double k2;
    };
