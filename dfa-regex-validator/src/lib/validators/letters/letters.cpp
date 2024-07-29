#include "letters.hpp"
#include "../../../../../exceptions/expression_exception.hpp"

letters::letters():validators("Letters"){}

bool letters::status0(const int c)
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool letters::is_letter(const int c, const bool alert)
{
    if(status0(c))
        return true;

    if (!alert)
        return false;

    throw expression_exception("Invalid letter. It must be an alphabetic character.");
}

bool letters::is_letter(const string* text, const bool alert)
{
    if (text->length() != 1)
    {
        if(!alert)
            return false;

        throw expression_exception("Invalid letter. Letter must be of length 1.");
    }

    return is_letter(text->at(0), alert);
}

bool letters::validate(const string* text, const bool alert)
{
    return is_letter(text, alert);
}
