#include "quadratic_isotherm.hpp"

#include <sstream>
#include <cmath>

QuadraticIsotherm::QuadraticIsotherm(double _q, double _k1, double _k2) :
    q {_q}, k1 {_k1}, k2 {_k2}
    {
    }

QuadraticIsotherm::~QuadraticIsotherm()
    {
    }

double
QuadraticIsotherm::loading(double p) const
    {
    return q * (k1 + 2.0 * k2 * p) * p / (1.0 + k1 * p + k2 * p * p);
    }

double
QuadraticIsotherm::spressure(double p) const
    {
    return q * std::log(1.0 + k1 * p + k2 * p * p);
    }

std::string
QuadraticIsotherm::getInfoString() const
    {
    std::stringstream ss;

    const char* expFormat =
        "# ==============================================================\n"
        "# Model Name: Quadratic Isotherm\n"
        "#\n"
        "#                           (k1 + 2 * k2 * P) * P  \n"
        "# Isotherm Form: n(P) = q * ---------------------  \n"
        "#                           1 + k1 * P + k2 * P^2  \n"
        "#\n"
        "# Henry Coefficient: q * k1\n"
        "# Saturation Loading: 2 * q\n"
        "#\n"
        "# Short name & Parameters:\n";

    ss << expFormat;

    ss << "quadratic" << '\n';
    for (const auto& e : getParameters())
        ss << e.first << "  " << e.second << '\n';

    return ss.str();
    }

Isotherm::ParameterType
QuadraticIsotherm::getParameters() const
    {
    ParameterType params;

    params["q"] = q;
    params["k1"] = k1;
    params["k2"] = k2;

    return params;
    } 
