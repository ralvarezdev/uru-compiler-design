#pragma once

#include<string>

using std::string;

// Tokens type enum
namespace tokens
{
    enum t_type
    {
        t_digits=0,
        t_floats,
        t_identifiers,
        t_integers,
        t_letters,
        t_operators,
        t_words,
        t_end
    };

    // Token type to string
    string to_string(t_type type);
}