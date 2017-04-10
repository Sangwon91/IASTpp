#pragma once

#include <istream>
#include <vector>
#include <string>
#include <tuple>
#include <memory>
#include <map>

#include "isotherm.hpp"

void readTwoColumns(const std::string& filename,
                    std::vector<double>& x,
                    std::vector<double>& y);

void readTwoColumns(std::istream& is,
                    std::vector<double>& x,
                    std::vector<double>& y);

double inverseIsotherm(Isotherm& isotherm, double n);

class IsothermModeler final
    {
public:
    enum Weight
        {
        ONE_OVER_Y = 0,
        UNITY
        };

    struct FitParameter
        {
        double initial;
        double min;
        double max;
        };

    using FitParameters = std::vector<FitParameter>;
    using ParameterMap = std::map<std::string, FitParameters>;

     IsothermModeler();
    ~IsothermModeler();

    std::shared_ptr<Isotherm> fit(const std::string& isoname,
                                  const std::vector<double>& x,
                                  const std::vector<double>& y,
                                  const Weight weight = ONE_OVER_Y);

    std::shared_ptr<Isotherm> autofit(
                                  const std::vector<double>& x,
                                  const std::vector<double>& y,
                                  const Weight weight = ONE_OVER_Y,
                                  const double minimumRsq = 0.9999);

    double getError() const;
    double getRSquare() const;

    void setParameter(const std::string& isoname, const FitParameters& params);
private:
    ParameterMap mParameterMap;
    std::map<std::string, int> mIsothermMap;
    double mError;
    double mRSquare;
    };
