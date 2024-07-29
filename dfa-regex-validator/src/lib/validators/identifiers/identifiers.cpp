#include "identifiers.hpp"
#include "../digits/digits.hpp"
#include "../letters/letters.hpp"
#include "../../../../../exceptions/expression_exception.hpp"

identifiers::identifiers():validators("Identifiers"){}

bool identifiers::is_extended_letter(const int c)
{
     return c=='_'||letters::is_letter(c);
}

bool identifiers::status0(const int c)
{
    return is_extended_letter(c);
}


bool identifiers::status1(const int c)
{
    return digits::is_digit(c)||is_extended_letter(c);
}

bool identifiers::is_identifier(const string* text, const bool alert)
{
    if (text->empty())
    {
        if(!alert)
            return false;

        throw expression_exception("Invalid identifier. Identifier must be at least of length 1.");
    }

    if(!status0(text->at(0)))
    {
        if(!alert)
            return false;

        throw expression_exception("Invalid identifier. Non-numeric characters must be either an alphabetic character or an underscore.");
    }

    if(text->length()==1)
        return true;

    for(int i=1;i<text->length();i++)
        if(!status1(text->at(i))){
            if(!alert)
                return false;

            throw expression_exception("Invalid identifier. Only alphabetic characters, digits, and underscores (at start) are allowed.");
        }

    return true;
}

bool identifiers::validate(const string* text,const bool alert)
{
    return is_identifier(text, alert);
}
