#include "interpolator_isotherm.hpp"

#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstddef>
#include <ciso646>

#include "isotherm_exception.hpp"
#include "linear_interpolator.hpp"

InterpolatorIsotherm::InterpolatorIsotherm(const std::vector<double>& x,
                                           const std::vector<double>& y)
    :
    mLoading {std::make_shared<LinearInterpolator>()},
    mSpressure {}
    {
    if (x.empty() or y.empty())
        {
        const char* msg {"Data is empty"};
        throw IsothermException {__FILE__, __LINE__, msg};
        }

    if (x.size() != y.size())
        {
        const char* msg {"x.size() != y.size()"};
        throw IsothermException {__FILE__, __LINE__, msg};
        }

    mLoading->setData(x, y);

    mSpressure.resize(x.size());
    mSpressure.front() = y.front();

    int maxi = static_cast<int>(x.size());
    for (int i = 1; i < maxi; ++i)
        {
        // y = a * x + b;
        double a = (y[i] - y[i - 1]) / (x[i] - x[i - 1]);
        double b = -a * x[i] + y[i];

        // integrate(y / x) = a * x + b * log(x) + C
        mSpressure[i] = mSpressure[i - 1] +
            a * (x[i] - x[i - 1]) + b * std::log(x[i] / x[i - 1]);
        }
    }

double
InterpolatorIsotherm::loading(double P) const
    {
    const auto& x = mLoading->getXData();
    const auto& y = mLoading->getYData();

    if (P < x.front())
        return y.front() / x.front() * P;

    return (*mLoading)(P);
    }

double
InterpolatorIsotherm::spressure(double P) const
    {
    const auto& xdata = mLoading->getXData();
    const auto& ydata = mLoading->getYData();

    std::size_t i =
        std::lower_bound(xdata.begin(), xdata.end(), P) - xdata.begin();

    if (i == 0 and P <= xdata.front())
        {
        const double x = xdata.front();
        const double y = ydata.front();

        return y / x * P;
        }

    if (i == mSpressure.size())
        {
        const double x = xdata.back();
        const double y = ydata.back();

        return mSpressure.back() + y * std::log(P / x);
        }

    const auto& x = xdata;
    const auto& y = ydata;

    double a = (y[i] - y[i - 1]) / (x[i] - x[i - 1]);
    double b = -a * x[i] + y[i];

    return mSpressure[i - 1] + a * (P - x[i - 1]) + b * std::log(P / x[i - 1]);
    }

std::string
InterpolatorIsotherm::getInfoString() const
    {
    std::stringstream ss;

    const auto& xdata = mLoading->getXData();
    const auto& ydata = mLoading->getYData();
    int size = xdata.size();

    const char* expFormat =
        "# ==============================================================\n"
        "# Model Name: Interpolator Isotherm\n"
        "#\n"
        "# Isotherm Form: n(P) = Linear Interpolator\n"
        "#\n"
        "# Henry Coefficient: Not available\n"
        "# Saturation Loading: Not available\n"
        "#\n"
        "# Short name & Row data:\n";

    ss << expFormat;
    ss << "interpolator" << '\n';
    for (int i = 0; i < size; ++i)
        ss << xdata[i] << "    " << ydata[i] << '\n';

    return ss.str();
    }

Isotherm::ParameterType
InterpolatorIsotherm::getParameters() const
    {
    ParameterType params;
    params["none"] = 0.0;

    return params;
    }

Interpolator&
InterpolatorIsotherm::getInterpolator()
    {
    return *mLoading;
    }

void
InterpolatorIsotherm::pushBack(double p, double n)
    {
    mLoading->pushBack(p, n);

    const auto& x = mLoading->getXData();
    const auto& y = mLoading->getYData();

    // last index.
    int i = x.size() - 1;

    // y = a * x + b;
    double a = (y[i] - y[i - 1]) / (x[i] - x[i - 1]);
    double b = -a * x[i] + y[i];

    // integrate(y / x) = a * x + log(x) + C
    mSpressure.push_back(mSpressure.back() +
        a * (x[i] - x[i - 1]) + b * std::log(x[i] / x[i - 1]));
    }
