#include "floats.hpp"

#include "../digits/digits.hpp"

#include "../../../../../exceptions/expressionException.hpp"

void floats::is_float(const string* text)
{
    bool is_whole_section = true;
    int counter = 0;

    for (const char c : *text)
    {
        if (c == '.')
        {
            if (is_whole_section)
            {
                is_whole_section = false;
                continue;
            }

            string msg = "Invalid float. Only one decimal point is allowed.";
            throw expression_exception(&msg);
        }

        try
        {
            digits::is_digit(c);
            counter++;
        }
        catch (expression_exception* _)
        {
            string msg = "Invalid float. Only digits and a decimal point are allowed.";
            throw expression_exception(&msg);
        }
    }

    if (!is_whole_section && counter == 0)
    {
        string msg = "Invalid float. At least one digit is required after decimal point.";
        throw expression_exception(&msg);
    }
}
