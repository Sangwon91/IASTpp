#include "isotherm_factory.hpp"

#include <functional>
#include <regex>
#include <stdexcept>
#include <locale>
#include <codecvt>

#include <ciso646>

#include "isotherm_exception.hpp"
#include "langmuir_isotherm.hpp"
#include "lf_isotherm.hpp"
#include "dsl_isotherm.hpp"
#include "dslf_isotherm.hpp"
#include "bet_isotherm.hpp"
#include "quadratic_isotherm.hpp"
#include "henry_isotherm.hpp"
#include "interpolator_isotherm.hpp"
#include "item_isotherm.hpp"

#include "isotherm_utility.hpp"

IsothermFactory::IsothermFactory()
    {
    mIsoMap["langmuir"]     = 2;
    mIsoMap["lf"]           = 3;
    mIsoMap["dsl"]          = 4;
    mIsoMap["dslf"]         = 6;
    mIsoMap["bet"]          = 3;
    mIsoMap["quadratic"]    = 3;
    mIsoMap["henry"]        = 1;
    mIsoMap["interpolator"] = 2;
    mIsoMap["item"]         = 4;
    }

std::shared_ptr<Isotherm>
IsothermFactory::create(const std::string& name, std::vector<Any> args) const
    {
    try {
        if (name == "langmuir")
            {
            double q1 = args[0].getAs<double>();
            double k1 = args[1].getAs<double>();

            return std::make_shared<LangmuirIsotherm>(q1, k1);
            }
        else if(name == "lf")
            {
            double q1 = args[0].getAs<double>();
            double k1 = args[1].getAs<double>();
            double n1 = args[2].getAs<double>();

            return std::make_shared<LfIsotherm>(q1, k1, n1);
            }
        else if (name == "dsl")
            {
            double q1 = args[0].getAs<double>();
            double k1 = args[1].getAs<double>();
            double q2 = args[2].getAs<double>();
            double k2 = args[3].getAs<double>();

            return std::make_shared<DslIsotherm>(q1, k1, q2, k2);
            }
        else if (name == "dslf")
            {
            double q1 = args[0].getAs<double>();
            double k1 = args[1].getAs<double>();
            double n1 = args[2].getAs<double>();
            double q2 = args[3].getAs<double>();
            double k2 = args[4].getAs<double>();
            double n2 = args[5].getAs<double>();

            return std::make_shared<DslfIsotherm>(q1, k1, n1, q2, k2, n2);
            }
        else if (name == "bet")
            {
            double q = args[0].getAs<double>();
            double k1 = args[1].getAs<double>();
            double k2 = args[2].getAs<double>();

            return std::make_shared<BetIsotherm>(q, k1, k2);
            }
        else if (name == "quadratic")
            {
            double q = args[0].getAs<double>();
            double k1 = args[1].getAs<double>();
            double k2 = args[2].getAs<double>();

            return std::make_shared<QuadraticIsotherm>(q, k1, k2);
            }
        else if (name == "henry")
            {
            double k = args[0].getAs<double>();

            return std::make_shared<HenryIsotherm>(k);
            }
        else if (name == "interpolator")
            {
            std::vector<double> x = args[0].getAs<std::vector<double>>();
            std::vector<double> y = args[1].getAs<std::vector<double>>();

            return std::make_shared<InterpolatorIsotherm>(x, y);
            }
        else if (name == "item")
            {
            auto isotherm = args[0].getAs< std::shared_ptr<Isotherm> >();
            Any {}.swap(args[0]); // Why?????????????????????????????????
            auto isoheat = args[1].getAs< std::function<double(double)> >();
            auto reftemp = args[2].getAs<double>();
            auto tartemp = args[3].getAs<double>();

            return std::make_shared<ItemIsotherm>(isotherm, isoheat, reftemp, tartemp);
            }
        else
            {
            throw IsothermException {__FILE__, __LINE__, name + ": Unsupported isotherm."};
            }
        }
    catch (AnyException& e)
        {
        throw IsothermException {__FILE__, __LINE__, "Invalid arguments type for " + name};
        }
    }

std::shared_ptr<Isotherm>
IsothermFactory::create(const std::string& isofile) const
    {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::ifstream ifs {converter.from_bytes(isofile.c_str())};

    if (not ifs.good())
        {
        std::string msg = "File " + isofile + "does not exist.";
        throw IsothermException {__FILE__, __LINE__, msg};
        }

    std::string isoname;
    std::regex whiteSpaced {R"-(\s*(.+)\s*)-"};;

    while (std::getline(ifs, isoname))
        if (isoname[0] != '#')
            break;

    std::smatch matched;
    std::regex_match(isoname, matched, whiteSpaced);

    isoname = matched[1];

    if (isoname == "interpolator")
        {
        std::string buffer;

        std::vector<double> x;
        std::vector<double> y;

        ::readTwoColumns(ifs, x, y);

        if (x.size() == 0 or y.size() == 0)
            {
            std::string msg = "No data in " + isofile;
            throw IsothermException {__FILE__, __LINE__, msg};
            }

        return std::make_shared<InterpolatorIsotherm>(x, y);
        }

    auto name = isoname;
    // Map with .at() to throw exception.
    try {
        auto para = readParameterMap(ifs);

        if (name == "langmuir")
            {
            double q = para.at("q");
            double k = para.at("k");

            return std::make_shared<LangmuirIsotherm>(q, k);
            }
        else if (name == "lf")
            {
            double q = para.at("q");
            double k = para.at("k");
            double n = para.at("n");

            return std::make_shared<LfIsotherm>(q, k, n);
            }
        else if (name == "dsl")
            {
            double q1 = para.at("q1");
            double k1 = para.at("k1");
            double q2 = para.at("q2");
            double k2 = para.at("k2");

            return std::make_shared<DslIsotherm>(q1, k1, q2, k2);
            }
        else if (name == "dslf")
            {
            double q1 = para.at("q1");
            double k1 = para.at("k1");
            double n1 = para.at("n1");
            double q2 = para.at("q2");
            double k2 = para.at("k2");
            double n2 = para.at("n2");

            return std::make_shared<DslfIsotherm>(q1, k1, n1, q2, k2, n2);
            }
        else if (name == "bet")
            {
            double q = para.at("q");
            double k1 = para.at("k1");
            double k2 = para.at("k2");

            return std::make_shared<BetIsotherm>(q, k1, k2);
            }
        else if (name == "quadratic")
            {
            double q = para.at("q");
            double k1 = para.at("k1");
            double k2 = para.at("k2");

            return std::make_shared<QuadraticIsotherm>(q, k1, k2);
            }
        else if (name == "henry")
            {
            double k = para.at("k");

            return std::make_shared<HenryIsotherm>(k);
            }
        else
            {
            throw IsothermException {__FILE__, __LINE__, name + ": Unsupported isotherm."};
            }
        }
    catch (const std::out_of_range& e)
        {
        std::string msg = "Invalid .isotherm syntax in " + isofile;

        throw IsothermException {__FILE__, __LINE__, msg};
        }
    }

IsothermFactory::ParameterType
IsothermFactory::readParameterMap(std::ifstream& ifs) const
    {
    std::string buffer;
    double dummy;

    ParameterType paramMap;

    while (ifs >> buffer >> dummy)
        paramMap[buffer] = dummy;

    return paramMap;
    }
