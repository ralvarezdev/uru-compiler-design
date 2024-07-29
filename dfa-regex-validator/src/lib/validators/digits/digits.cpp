#include<string>

#include "digits.hpp"

#include "../../../../../exceptions/expression_exception.hpp"

using std::string;

digits::digits(): validators("Digits")
{
}

bool digits::status0(const int c)
{
    return isdigit(c);
}

bool digits::is_digit(const int c, const bool alert)
{
    if (status0(c))
        return true;

    if (!alert)
        return false;

    throw expression_exception("Invalid character. Only digits are allowed.");
}

bool digits::is_digit(const string* text, const bool alert)
{
    if (text->length() != 1)
    {
        if (!alert)
            return false;

        throw expression_exception("Invalid digit. Digit must be of length 1.");
    }

    return is_digit(text->at(0), alert);
}

bool digits::validate(const string* text, const bool alert)
{
    return is_digit(text, alert);
}
