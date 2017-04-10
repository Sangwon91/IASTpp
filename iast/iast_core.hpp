#pragma once

#include "isotherm.hpp"

#include <vector>
#include <memory>
#include <tuple>

#include "iast_exception.hpp"

class Iast
    {
public:
    using ValueType       = double;
    using IsothermVector  = std::vector< std::shared_ptr<Isotherm> >;
    using ResultType = std::tuple< ValueType, std::vector<ValueType> >;

    enum Mode {FIX_PY, FIX_PX, FIX_NX};

     Iast() = default;
    ~Iast() = default;

    // Operators.
    std::shared_ptr<Isotherm>& operator [] (int i); // Direct access to i'th isotherm.

    // Setters.
    Iast& setIsotherms(IsothermVector isothermVector);

    // Methods.
    Iast& calculate(int mode, ValueType pOrN, std::vector<ValueType> composition);

    // Getters.
    ResultType getResult() const;
private:
    IsothermVector mIsotherms;
    ResultType mResult;

    void modeFixPy(ValueType p, std::vector<ValueType> y);
    void modeFixPx(ValueType p, std::vector<ValueType> x);
    void modeFixNx(ValueType n, std::vector<ValueType> x);
    };
