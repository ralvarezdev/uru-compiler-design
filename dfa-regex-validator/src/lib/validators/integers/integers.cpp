#include "integers.hpp"

#include "../digits/digits.hpp"

#include "../../../../../exceptions/expressionException.hpp"

void integers::is_integer(const string* text)
{
    if (text->empty())
    {
        string msg = "Invalid integer. Integer must be at least of length 1.";
        throw expression_exception(&msg);
    }

    try{
        for(const char c : *text)
            digits::is_digit(c);
    }

    catch(expression_exception* _)
    {
        string msg = "Invalid integer. Only digits are allowed.";
        throw expression_exception(&msg);
    }
}
