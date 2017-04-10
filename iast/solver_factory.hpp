#pragma once

#include <memory>
#include <string>
#include "solver.hpp"

class SolverFactory
    {
public:
             SolverFactory() = default;
    virtual ~SolverFactory() = default;

    virtual std::shared_ptr<Solver> create(std::string name);
    };
