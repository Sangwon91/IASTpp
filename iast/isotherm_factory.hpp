#pragma once

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <fstream>

#include "isotherm.hpp"
#include "any.hpp"

class IsothermFactory
    {
public:
    using ParameterType = Isotherm::ParameterType;

     IsothermFactory();
    ~IsothermFactory() = default;

    std::shared_ptr<Isotherm> create(const std::string& name, std::vector<Any> args) const;
    std::shared_ptr<Isotherm> create(const std::string& isofile) const;
private:
    // Isotherm name & # of parameters(or arguments).
    std::unordered_map<std::string, int> mIsoMap;
    ParameterType readParameterMap(std::ifstream& ifs) const;
    };
