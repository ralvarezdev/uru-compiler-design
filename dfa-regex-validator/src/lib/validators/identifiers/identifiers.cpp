#include "identifiers.hpp"

#include "../digits/digits.hpp"

#include "../../../../../exceptions/expressionException.hpp"

bool identifiers::is_letter(const int c,const bool alert)
{
    if(c=='_'||letters::is_letter(c))
        return true;

    if(!alert)
        return false;

    string msg = "Invalid identifier. Non-numeric characters must be either an alphabetic character or an underscore.";
    throw expression_exception(&msg);
}

void identifiers::is_identifier(const string* text)
{
    if (text->empty())
    {
        string msg = "Invalid identifier. Identifier must be at least of length 1.";
        throw expression_exception(&msg);
    }

    unsigned char c= text->at(0);
    identifiers::is_letter(c, true);

    if(text->length()==1)
        return;

    for (c : text->substr(1))
        if(!digits::is_digit(c)&&!identifiers::is_letter(c)))
        {
            string msg = "Invalid identifier. Only alphabetic characters, digits, and underscores (at start) are allowed.";
            throw expression_exception(&msg);
        }
}
