#include "isotherm_information.h"

IsothermInformation::IsothermInformation(std::string fullName,
    std::string shortName, int numParams,
    std::vector<std::string> paramNames, bool isModel) :
    mFullName (fullName),
    mShortName (shortName),
    mNumParams (numParams),
    mParamNames (paramNames),
    mIsModel (isModel)
{

}

std::string
IsothermInformation::fullName() const
    {
    return mFullName;
    }

std::string
IsothermInformation::shortName() const
    {
    return mShortName;
    }

int
IsothermInformation::numParams() const
    {
    return mNumParams;
    }

std::vector<std::string>
IsothermInformation::paramNames() const
    {
    return mParamNames;
    }

bool
IsothermInformation::isModel() const
    {
    return mIsModel;
    }
