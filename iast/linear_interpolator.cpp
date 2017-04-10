#include "linear_interpolator.hpp"

#include <algorithm>
#include <cstddef>
#include <ciso646>

double
LinearInterpolator::at(double x) const
    {
    //if (xs.empty())
    //    ;

    std::size_t i = std::lower_bound(xs.begin(), xs.end(), x) - xs.begin();

    if (i == 0)
        return ys.front();

    if (i == xs.size())
        return ys.back();

    double slope = (ys[i] - ys[i - 1]) /
                   (xs[i] - xs[i - 1]);

    return slope * (x - xs[i]) + ys[i];
    }

std::vector<double>&
LinearInterpolator::getXData()
    {
    return xs;
    }

std::vector<double>&
LinearInterpolator::getYData()
    {
    return ys;
    }

double
LinearInterpolator::operator () (double x) const
    {
    return this->at(x);
    }

Interpolator&
LinearInterpolator::setData(const std::vector<double>& x,
                            const std::vector<double>& y)
    {
    xs = x;
    ys = y;

    return *this;
    }

Interpolator&
LinearInterpolator::pushBack(double x, double y)
    {
    xs.push_back(x);
    ys.push_back(y);

    return *this;
    }

Interpolator&
LinearInterpolator::setOption(int option, double value)
    {
    option = option;
    value = value;
    // throw
    return *this;
    }
