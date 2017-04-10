#include "lf_isotherm.hpp"

#include <cmath>
#include <sstream>

LfIsotherm::LfIsotherm(double q, double k, double n) :
    mQ {q}, mK {k}, mN {n}
    {

    }

double
LfIsotherm::loading(double P) const
    {
    double kpn = std::pow(mK * P, mN);
    return mQ * kpn / (1.0 + kpn);
    }

double
LfIsotherm::spressure(double P) const
    {
    double kpn = std::pow(mK * P, mN);
    return mQ / mN * std::log(1.0 + kpn);
    }

std::string
LfIsotherm::getInfoString() const
    {
    std::stringstream ss;

    const char* expFormat =
        "# ==============================================================\n"
        "# Model Name: Langmuir-Freundlich Isotherm\n"
        "#\n"
        "#                             (k * P)^n    \n"
        "# Isotherm Form: n(P) = q * -------------  \n"
        "#                           1 + (k * P)^n  \n"
        "#\n"
        "# Henry Coefficient: Not available\n"
        "# Saturation Loading: q\n"
        "#\n"
        "# Short name & Parameters:\n";

    ss << expFormat;

    ss << "lf" << '\n';
    for (const auto& e : getParameters())
        ss << e.first << "  " << e.second << '\n';

    return ss.str();
    }


Isotherm::ParameterType
LfIsotherm::getParameters() const
    {
    ParameterType params;

    params["q"] = mQ;
    params["k"] = mK;
    params["n"] = mN;

    return params;
    }
