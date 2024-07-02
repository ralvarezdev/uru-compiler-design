#include<algorithm>

#include "integers.hpp"
#include "../digits/digits.hpp"
#include "../../../../../exceptions/expressionException.hpp"

integers::integers():validators("Integers"){}

bool integers::status0(const int c)
{
    return digits::is_digit(c);
}

bool integers::is_integer(const string* text, const bool alert)
{
    if (text->empty())
    {
        if(!alert)
            return false;

        throw expression_exception("Invalid integer. Integer must be at least of length 1.");
    }

    if(const vector<char> characters=get_characters(text);
        std::ranges::all_of(characters.cbegin(), characters.cend(),[](const char c){ return status0(c); }))
        return true;

    if(!alert)
        return false;

    throw expression_exception("Invalid integer. Only digits are allowed.");
}

bool integers::validate(const string* text,const bool alert)
{
    return is_integer(text, alert);
}
