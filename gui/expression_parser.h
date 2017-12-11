#ifndef EXPRESSIONPARSER_H
#define EXPRESSIONPARSER_H

#include <string>
#include <memory>
#include <unordered_map>

class ExpressionParser
{
public:
    using VariableMap = std::unordered_map<std::string, double>;

    ExpressionParser();
    ~ExpressionParser();

    void setExpression(const std::string& expression);
    double evaluate(VariableMap& vMap = EmptyMap);

private:
    static VariableMap EmptyMap;
    // To hide implementation details.
    struct impl;
    std::unique_ptr<impl> pImpl;
};

#endif // EXPRESSIONPARSER_H
