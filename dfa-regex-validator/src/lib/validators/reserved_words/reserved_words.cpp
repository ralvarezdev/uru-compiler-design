#include<string>

#include "reserved_words.hpp"

#include "../../../../../exceptions/expression_exception.hpp"

using std::string;

reserved_words::reserved_words(): validators("reserved_words")
{
}

bool reserved_words::is_reserved_word(const string* text, const bool alert)
{
    for(const auto& reserved_word : r_words::reserved_words)
        if(*text == reserved_word)
            return true;

    if(alert)
        throw expression_exception("Invalid word. Only reserved words are allowed.");

    return false;
}

bool reserved_words::validate(const string* text, const bool alert)
{
    return is_reserved_word(text, alert);
}
