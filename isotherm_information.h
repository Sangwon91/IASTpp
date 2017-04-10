#ifndef ISOTHERMINFORMATION_H
#define ISOTHERMINFORMATION_H

// C Like simple data structure

#include <string>
#include <vector>

class IsothermInformation
{
public:
    IsothermInformation(std::string fullName, std::string shortName, int numParams,
                        std::vector<std::string> paramNames, bool isModel);

    std::string fullName() const;
    std::string shortName() const;
    int numParams() const;
    std::vector<std::string> paramNames() const;
    bool isModel() const;
private:
    std::string mFullName;
    std::string mShortName;
    int mNumParams;
    std::vector<std::string> mParamNames;
    bool mIsModel;
};

#endif // ISOTHERMINFOMATION_H
