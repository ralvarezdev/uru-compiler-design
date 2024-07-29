#include "tokens.hpp"

#include<sstream>

using std::stringstream;

// - Tokens namespace

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

// - Token info class

// Constructor
token_info::token_info(map<tokens::t_type, bool>* type,const int column)
{
    this->type_ = type;
    this->column_ = column;
}

// To string
string token_info::to_string()
{
    stringstream msg;

    msg<<"Token types: ";
    for(auto const& [key,value] : *this->type_)
    {
        if(value)
            msg<<"'"<<tokens::to_string(key)<<"' ";
    }
    msg<<"\n"
        <<"Token column: '"<<this->column_<<"'\n";

    return msg.str();
}

// - Token class

// Constructor
token::token(const string& key, token_info* info)
{
    this->key_ = key;
    this->info_ = info;
}

// To string
string token::to_string()
{
    stringstream msg;

    msg<<"Token name: "<<this->key_<<"\n";
    msg<<this->info_->to_string();

    return msg.str();
}