#include "tokens.hpp"

// Token type to string
string tokens::to_string(t_type type)
{
    switch(type)
    {
    case t_digits:
        return "digit";
    case t_floats:
        return "float";
    case t_identifiers:
        return "identifier";
    case t_integers:
        return "integer";
    case t_letters:
        return "letter";
    case t_operators:
        return "operator";
    case t_words:
        return "word";
    default:
        return "undefined";
    }
}
