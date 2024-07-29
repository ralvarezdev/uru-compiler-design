#include<string>
#include<vector>
#include<format>

#include "operators.hpp"

#include <bits/ranges_algo.h>

#include "../../../../../exceptions/expressionException.hpp"

using std::string;
using std::to_string;
using std::vector;
using std::format;

operators::operators(): validators("Operators")
{
}

bool operators::status0(const int c, const int* valid_op)
{
    return std::ranges::any_of(valid_op, valid_op + 9, [c](const int op) { return c == op; });
}

bool operators::is_operator(const int c, const bool alert)
{
    // List of valid operators
    constexpr int valid_op[]{'+', '-', '*', '/', '%', '^', '(', ')', '='};

    if (status0(c, valid_op))
        return true;

    if (!alert)
        return false;

    string valid_op_msg = format("'{}'", valid_op[0]);

    for (int valid = 1; valid < 5; ++valid)
        valid_op_msg += format(", '{}'", valid);

    const char* msg = format("Invalid operator. Only {} are allowed", valid_op_msg).c_str();
    throw expression_exception(msg);
}

bool operators::is_operator(const string* text, const bool alert)
{
    if (text->length() != 1)
    {
        if (!alert)
            return false;

        throw expression_exception("Invalid operator. Operator must be of length 1.");
    }

    return is_operator(text->at(0), alert);
}


bool operators::validate(const string* text, const bool alert)
{
    return is_operator(text, alert);
}
