#include "expression_parser.h"

#include <exception>
#include <stdexcept>

#include "muparser/muParser.h"

ExpressionParser::VariableMap ExpressionParser::EmptyMap;

struct ExpressionParser::impl
    {
     impl() = default;
    ~impl() = default;

    mu::Parser parser;
    };

ExpressionParser::ExpressionParser() : pImpl {new impl}
    {

    }

// To use pimpl ediom
ExpressionParser::~ExpressionParser()
    {

    }

void
ExpressionParser::setExpression(const std::string& expression)
    {
    (pImpl->parser).SetExpr(expression);
    }

double
ExpressionParser::evaluate(VariableMap& vMap)
    {
    double returnValue;

    try {
        // It's more safe design.
        // Because it sets the pointer of the variables when you calculate.
        // No deleted pointer is expected.
        (pImpl->parser).ClearVar();

        for (auto& v : vMap)
            (pImpl->parser).DefineVar(v.first, &v.second);

        returnValue = (pImpl->parser).Eval();
        }
    catch (mu::Parser::exception_type& e)
        {
        // Convert to general exception.
        // No return here.
        throw std::runtime_error {e.GetMsg().c_str()};
        }

    return returnValue;
    }
