#pragma once

#include <string>
#include <unordered_map>

class Isotherm
    {
public:
    using ParameterType = std::unordered_map<std::string, double>;

    virtual ~Isotherm() = default;

    virtual double loading(double P) const = 0;
    virtual double spressure(double P) const = 0;

    virtual std::string getInfoString() const = 0;
    virtual ParameterType getParameters() const = 0;
    };
