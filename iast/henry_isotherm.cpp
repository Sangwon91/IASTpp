#include "henry_isotherm.hpp"

#include <cmath>
#include <sstream>

HenryIsotherm::HenryIsotherm(double k) :
    mK {k}
    {

    }

double
HenryIsotherm::loading(double P) const
    {
    return  mK * P;
    }

double
HenryIsotherm::spressure(double P) const
    {
    return mK * P;
    }

std::string
HenryIsotherm::getInfoString() const
    {
    std::stringstream ss;

    const char* expFormat =
        "# ==============================================================\n"
        "# Model Name: Henry (Linear) Isotherm\n"
        "#\n"
        "# Isotherm Form: n(P) = k * P\n"
        "#\n"
        "# Henry Coefficient: k\n"
        "# Saturation Loading: Not available\n"
        "#\n"
        "# Short name & Parameters:\n";

    ss << expFormat;
    ss << "henry" << '\n';
    for (const auto& e : getParameters())
        ss << e.first << "  " << e.second << '\n';

    return ss.str();
    }

Isotherm::ParameterType
HenryIsotherm::getParameters() const
    {
    ParameterType params;
    params["k"] = mK;

    return params;
    }
