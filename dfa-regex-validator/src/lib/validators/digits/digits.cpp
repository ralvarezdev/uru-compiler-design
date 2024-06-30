#include<string>

#include "digits.hpp"

#include "../../../../../exceptions/expressionException.hpp"

using std::string;

bool digits::is_digit(const int c, const bool alert)
{
    if(isdigit(c))
        return true;

    if(!alert)
        return false;

    string msg ="Invalid whole number. Only digits are allowed.";
    throw expression_exception(&msg);
}


void digits::is_digit(const string* text){
    if(text->length()!=1)
    {
        string msg ="Invalid digit. Digit must be of length 1.";
        throw expression_exception(&msg);
    }

    const unsigned char c= text->at(0);

    is_digit(c, true);
}
