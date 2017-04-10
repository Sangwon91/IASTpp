#include "item_isotherm.hpp"

#include <sstream>
#include <iomanip>
#include <cmath>
#include <ciso646>

#include "isotherm_exception.hpp"
#include "isotherm_utility.hpp"

ItemIsotherm::ItemIsotherm(IsothermPtr isotherm,
                           FunctorType isoheat,
                           double refTemperature,
                           double tarTemperature)
    try :
        mRefIsotherm {isotherm},
        mIsoheat {isoheat},
        mRefTemperature {refTemperature},
        mTarTemperature {tarTemperature},
        mIsotherm {{newPressure(0.1)}, {0.1}}
        {
        // Body
        }
    catch (IsothermException& e)
        {
        std::string msg {"Item construction fails."};
        msg += "\n    Reason: ";
        msg += e.what();
        throw IsothermException {__FILE__, __LINE__, msg};
        }

ItemIsotherm::~ItemIsotherm()
    {
    // Why????????????????????
    // Why Explicit Define????
    }

double
ItemIsotherm::loading(double p) const
    {
    this->expand(p);

    return mIsotherm.loading(p);
    }

double
ItemIsotherm::spressure(double p) const
    {
    this->expand(p);

    return mIsotherm.spressure(p);
    }

std::string
ItemIsotherm::getInfoString() const
    {
    std::stringstream ss;

    auto& xdata = mIsotherm.getInterpolator().getXData();
    auto& ydata = mIsotherm.getInterpolator().getYData();

    int size = xdata.size();

    for (int i = 0; i < size - 1; ++i)
        ss << std::setw(15) << xdata[i] << std::setw(15) << ydata[i] << '\n';
    ss << std::setw(15) << xdata.back() << std::setw(15) << ydata.back();

    return ss.str();
    }

Isotherm::ParameterType
ItemIsotherm::getParameters() const
    {
    ParameterType params;

    params["ref"] = mRefTemperature;
    params["tar"] = mTarTemperature;

    return params;
    }

void
ItemIsotherm::expand(double p) const
    {
    try {
        auto& xdata = mIsotherm.getInterpolator().getXData();
        auto& ydata = mIsotherm.getInterpolator().getYData();

        if (xdata.empty())
            {
            double newn = 0.1;
            double tarp = newPressure(0.1);

            mIsotherm.pushBack(tarp, newn);
            }

        while (p > xdata.back())
            {
            double newn = ydata.back() + 0.1;
            double tarp = newPressure(newn);

            mIsotherm.pushBack(tarp, newn);
            }
        }
    catch (IsothermException& e)
        {
        std::string msg {"Expand fails."};
        msg += "\n    Reason: ";
        msg += e.what();
        throw IsothermException {__FILE__, __LINE__, msg};
        }
    }

double
ItemIsotherm::newPressure(double n) const
    {
    double R = 0.008314469;
    double dbeta = (1.0 / mTarTemperature - 1.0 / mRefTemperature) / R;

    double p = inverseIsotherm(*mRefIsotherm, n) * std::exp(-dbeta * mIsoheat(n));

    return p;
    }
