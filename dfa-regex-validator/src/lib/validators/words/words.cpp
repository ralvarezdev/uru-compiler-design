#include <algorithm>

#include "words.hpp"
#include "../letters/letters.hpp"
#include "../../../../../exceptions/expressionException.hpp"

words::words(): validators("Words")
{
}

bool words::status0(const int c)
{
    return letters::is_letter(c);
}


bool words::is_word(const string* text, const bool alert)
{
    if (text->empty())
    {
        if (!alert)
            return false;

        throw expression_exception("Invalid word. Word must be at least of length 1.");
    }

    if (const vector<char> characters = get_characters(text);
        std::ranges::all_of(characters.cbegin(), characters.cend(), [](const char c) { return status0(c); }))
        return true;

    if (!alert)
        return false;

    throw expression_exception("Invalid word. Only alphabetic characters are allowed.");
}

bool words::validate(const string* text, const bool alert)
{
    return is_word(text, alert);
}
