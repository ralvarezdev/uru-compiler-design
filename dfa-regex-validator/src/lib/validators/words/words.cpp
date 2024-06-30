#include "words.hpp"

#include "../letters/letters.hpp"

#include "../../../../../exceptions/expressionException.hpp"

void words::is_word(const string* text)
{
    if (text->empty())
    {
        string msg = "Invalid word. Word must be at least of length 1.";
        throw expression_exception(&msg);
    }

    try{
        for(const char c : *text)
            letters::is_letter(c);
    }

    catch(expression_exception* _)
    {
        string msg = "Invalid word. Only alphabetic characters are allowed.";
        throw expression_exception(&msg);
    }
}
