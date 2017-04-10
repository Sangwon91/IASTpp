#include "simplex.hpp"

#include <algorithm>
#include <limits>
#include <cmath>
#include <ciso646>

Simplex::Simplex() :
    Minimizer {},
    mNumRepeats {5},
    mTolX {1.0e-8},
    mTolF {1.0e-8},
    x {mPoints},
    f {mValues},
    n {mDimension}
    {

    }

Simplex::~Simplex()
    {

    }

Minimizer&
Simplex::setFunction(FunctionType func)
    {
    mObjectiveCoreFunction = func;
    mObjective = [this](const VectorType& vec)
        {
        PointType p;
        p.resize(n);

        for (int i = 0; i < n; ++i)
            p[i] = vec[i];

        return mObjectiveCoreFunction(p);
        };

    return *this;
    }

Minimizer&
Simplex::setInitialPoint(PointType point)
    {
    // Set dimension.
    n = point.size();
    x.resize(n + 1);
    x[0].resize(n);

    for (int i = 0; i < n; ++i)
        x[0][i] = point[i];

    return *this;
    }

Minimizer&
Simplex::setOption(int option, ValueType value)
    {
    switch (option)
        {
        case NUM_REPEATS:
            mNumRepeats = static_cast<int>(value + 1.0e-10);
            break;
        case TOL_X:
            mTolX = value;
            break;
        case TOL_F:
            mTolF = value;
            break;
        default:
            throw MinimizerException {__FILE__, __LINE__, "Unsupported option."};
            break;
        }

    return *this;
    }

Minimizer&
Simplex::minimize()
    {
    int numFunctionCalls = 0;
    this->minimizeOnce();
    numFunctionCalls += this->getNumFunctionCalls();

    for (int i = 1; i < mNumRepeats; ++i)
        {
        this->setInitialPoint(this->getMinimumPoint());
        this->minimizeOnce();
        numFunctionCalls += this->getNumFunctionCalls();
        }

    mNumFunctionCalls = numFunctionCalls;

    return *this;
    }

void
Simplex::minimizeOnce()
    {
    f.resize(n + 1);
    f[0] = mObjective(x[0]);

    for (int i = 1; i < n + 1; ++i)
        {
        x[i].resize(n);
        x[i] = x[0];
        x[i][i - 1] *= 1.5;

        f[i] = mObjective(x[i]);
        }
    // Sort points and values with values.
    this->sort();
    this->center(); // xo
    xr.resize(n, 0.0);
    xe.resize(n, 0.0);
    xc.resize(n, 0.0);
    fr = 0.0;
    fe = 0.0;
    fc = 0.0;

    // Main routine start.
    mNumFunctionCalls = 0;
    while (mNumFunctionCalls < 1000)
        {
        if (not this->reflect())
            {
            if (fr < f[0])
                {
                this->expand();
                }
            else
                {
                if (not this->contract())
                    this->shrink();
                }
            }

        if (this->check())
            break;
        }
    }

Minimizer::PointType
Simplex::getMinimumPoint() const
    {
    Minimizer::PointType p;
    p.resize(n);

    for (int i = 0; i < n; ++i)
        p[i] = x[0][i];

    return p;
    }

Minimizer::ValueType
Simplex::getMinimumValue() const
    {
    return f[0];
    }

int
Simplex::getNumFunctionCalls() const
    {
    return mNumFunctionCalls;
    }

bool
Simplex::reflect()
    {
    bool isUpdated = false;

    xr = xo + 1.0 * (xo - x[n]);
    fr = mObjective(xr);
    mNumFunctionCalls++;

    if (f[0] <= fr and fr <= f[n - 1])
        {
        x[n] = xr;
        f[n] = fr;

        this->sort();
        this->center();

        isUpdated = true;
        }

    return isUpdated;
    }

bool
Simplex::expand()
    {
    xe = xo + 2.0 * (xr - xo);
    fe = mObjective(xe);
    mNumFunctionCalls++;

    if (fe < fr)
        {
        x[n] = xe;
        f[n] = fe;
        }
    else
        {
        x[n] = xr;
        f[n] = fr;
        }

    this->sort();
    this->center();

    return true;
    }

bool
Simplex::contract()
    {
    bool isUpdated = false;

    xc = xo + 0.5 * (x[n] - xo);
    fc = mObjective(xc);
    mNumFunctionCalls++;

    if (fc < f[n])
        {
        x[n] = xc;
        f[n] = fc;

        this->sort();
        this->center();

        isUpdated = true;
        }

    return isUpdated;
    }

bool
Simplex::shrink()
    {
    for (int i = 1; i < n + 1; ++i)
        {
        x[i] = x[0] + 0.5 * (x[i] - x[0]);
        f[i] = mObjective(x[i]);
        mNumFunctionCalls++;
        }

    return true;
    }

bool
Simplex::check()
    {
    const double tolf = mTolF;
    const double tolx = mTolX;
    const double eps = std::numeric_limits<double>::epsilon();
    double delta = std::abs(f[n] - f[0]);
    double accuracy = (std::abs(f[n]) + std::abs(f[0])) * tolf;

    return delta < (accuracy + eps) and std::abs(x[0] - x[n]).sum() < tolx;
    }

void
Simplex::sort()
    {
    bool isSwapped = true;
    while (isSwapped)
        {
        isSwapped = false;
        for (int i = 0; i < n; ++i)
            {
            if (f[i] > f[i + 1])
                {
                std::swap(f[i], f[i + 1]);
                x[i].swap(x[i + 1]);
                isSwapped = true;
                }
            }
        }
    }

void
Simplex::center()
    {
    xo.resize(n, 0.0);
    // Note x[n] is excepted!
    for (int i = 0; i < n; ++i)
        xo += x[i];

    xo /= static_cast<double>(n);
    }
