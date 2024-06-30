#include "letters.hpp"

#include "../../../../../exceptions/expressionException.hpp"

bool letters::is_letter(const int c, const bool alert)
{
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
        return true;

    if (!alert)
        return false;

    string msg = "Invalid letter. It must be an alphabetic character.";
    throw expression_exception(&msg);
}

void letters::is_letter(const string* text)
{
    if (text->length() != 1)
    {
        string msg = "Invalid letter. Letter must be of length 1.";
        throw expression_exception(&msg);
    }

    const unsigned char c = text->at(0);

    is_letter(c);
}
