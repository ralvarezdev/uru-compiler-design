#include<string>
#include<vector>
#include<format>

#include "operators.hpp"

#include "../../../../../exceptions/expressionException.hpp"

using std::string;
using std::to_string;
using std::vector;
using std::format;

bool operators::is_operator(const int c, const bool alert)
{
    // List of valid operators
    const vector<int> valid_op = {'+', '-', '*', '/', '%'};

    for (const int valid : valid_op)
        if (c == valid)
            return true;

    if (!alert)
        return false;

    string valid_op_msg = format("'{}'", valid_op.front());

    for (const int valid : valid_op)
        valid_op_msg += format(", '{}'", valid);

    string msg = format("Invalid operator. Only {} are allowed", valid_op_msg);
    throw expression_exception(&msg);
}


void operators::is_operator(const string* text)
{
    if (text->length() != 1)
    {
        string msg = "Invalid operator. Operator must be of length 1.";
        throw expression_exception(&msg);
    }

    const unsigned char op = text->at(0);

    is_operator(op, true);
}
