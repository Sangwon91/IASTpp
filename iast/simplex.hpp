#pragma once

#include "minimizer.hpp"

#include <valarray>
#include <vector>
#include <functional>

class Simplex : public Minimizer
    {
public:
    enum Option : int {NUM_REPEATS, TOL_X, TOL_F};

              Simplex();
    virtual  ~Simplex();
    virtual Minimizer& minimize();

    virtual Minimizer& setFunction(FunctionType func) override;
    virtual Minimizer& setInitialPoint(PointType point) override;
    virtual Minimizer& setOption(int option, ValueType value) override;

    virtual PointType  getMinimumPoint() const override;
    virtual ValueType  getMinimumValue() const override;
    virtual int        getNumFunctionCalls() const override;
private:
    using VectorType  = std::valarray<ValueType>;
    using VectorsType = std::vector<VectorType>;
    using ValuesType  = std::vector<ValueType>;
    using VectorFunctionType = std::function< double(const VectorType&) >;

    int          mNumFunctionCalls;
    int          mDimension;
    int          mNumRepeats;
    ValueType    mTolX;
    ValueType    mTolF;
    VectorsType  mPoints;
    ValuesType   mValues;
    FunctionType       mObjectiveCoreFunction;
    VectorFunctionType mObjective;

    // Wikipedia notation.
    // Wiki: Nelder-Mead method.
    VectorType xo; // Center of Simplex except x[n].
    VectorType xr; // Reflected Point.
    VectorType xe; // Expanded Point.
    VectorType xc; // Contracted Point.

    ValueType fr; // Value of Reflected Point.
    ValueType fe; // Value of Expanded Point.
    ValueType fc; // Value of Contracted Point.

    VectorsType& x;
    ValuesType&  f;
    int& n;

    void minimizeOnce();

    bool reflect();
    bool expand();
    bool contract();
    bool shrink();
    bool check(); // Check convergence.

    void sort();
    void center();
    };
