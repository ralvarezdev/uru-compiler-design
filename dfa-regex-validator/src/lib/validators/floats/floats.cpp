#include "floats.hpp"
#include "../digits/digits.hpp"
#include "../../../../../exceptions/expressionException.hpp"

floats::floats():validators("Floats"){}

bool floats::status0(const int c)
{
    return digits::is_digit(c);
}

bool floats::status1(const int c)
{
    return c=='.';
}

bool floats::status2(const int c)
{
    return digits::is_digit(c);
}


bool floats::is_float(const string* text, const bool alert)
{
    int status = 0;
    int counter = 0;

    for (const char c : *text)
    {
        if (status1(c))
        {
            if (status==0&&counter>0)
            {
                status+=2;
                counter=0;
                continue;
            }

            if(!alert)
                return false;

            throw expression_exception("Invalid float. Only one decimal point is allowed.");
        }

        if((status==0&&status0(c))||(status==2&&status2(c)))
        {
            counter++;
            continue;
        }

        if(!alert)
            return false;

        throw expression_exception("Invalid float. Only digits and a decimal point are allowed.");
    }

    if (status<2 || counter == 0)
    {
        if(!alert)
            return false;

        throw expression_exception("Invalid float. At least one digit is required after decimal point.");
    }

    return true;
}

bool floats::validate(const string* text, const bool alert)
{
    return is_float(text, alert);
}
