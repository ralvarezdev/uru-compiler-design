#include "tokens.hpp"

#include<sstream>

using std::stringstream;

// - Tokens namespace

// Token type to string
string tokens::to_string(t_type type)
{
    switch(type)
    {
        /*
    case t_digits:
        return "digit";
        */
    case t_integers:
        return "integer";
    case t_floats:
        return "float";
    case t_identifiers:
        return "identifier";
    /*
    case t_letters:
        return "letter";
        */
    case t_reserved_words:
        return "reserved word";
    case t_operators:
        return "operator";
    /*
    case t_words:
        return "word";
        */
    default:
        return "undefined";
    }
}

// - Token info class

// Constructor
token_info::token_info(map<tokens::t_type, bool>* type,const int column, string& key)
{
    this->is_undefined_=!type;
    this->column_ = column;

    if(this->is_undefined_)
        return;

    this->type_ = type;
    this->data_=map<tokens::t_type, token_data*>();

    // Cast key
    this->cast_key(key);
}

// Temporary token info constructor
token_info::token_info(const tokens::t_type t_type, const float number)
{
    this->type_=generate_type(t_type);
    this->data_=map<tokens::t_type, token_data*>();

    for(int i=0; i<tokens::t_end; i++)
    {
        const auto type=static_cast<tokens::t_type>(i);
        this->data_[type]=(type==t_type)?new token_data_float(number):nullptr;
    }
}

token_info::token_info(const tokens::t_type t_type,const  int number)
{
    this->type_=generate_type(t_type);
    this->data_=map<tokens::t_type, token_data*>();

    for(int i=0; i<tokens::t_end; i++)
    {
        const auto type=static_cast<tokens::t_type>(i);
        this->data_[type]=(type==t_type)?new token_data_int(number):nullptr;
    }
}

// Generate type
map<tokens::t_type, bool>* token_info::generate_type(const tokens::t_type t_type)
{
    const auto type = new map<tokens::t_type, bool>();

    for(int i=0; i<tokens::t_end; i++)
        (*type)[static_cast<tokens::t_type>(i)]= i==t_type;

    return type;
}

// Cast token key string
void token_info::cast_key(string& key)
{
    // Store possible values
    for(int i=0; i<tokens::t_end; i++)
    {
        const auto t_type=static_cast<tokens::t_type>(i);

        // Check if type is valid
        if(!this->type_->at(t_type))
        {
            this->data_[t_type]=nullptr;
            continue;
        }

        switch(t_type)
        {
        case tokens::t_identifiers:
        case tokens::t_operators:
        case tokens::t_reserved_words:
            this->data_[t_type]=new token_data_string(key);
            break;

        case tokens::t_integers:
            this->data_[t_type]=new token_data_int(stoi(key));
            break;

        case tokens::t_floats:
            this->data_[t_type]=new token_data_float(stof(key));
            break;

        default:
            break;
        }
    }
}

// To string
string token_info::to_string()
{
    stringstream msg;
    token_data* t_data;

    msg<<"Token column: '"<<this->get_column()<<"'\n"
        <<"Token types: ";

    // Check if it's undefined
    if(this->is_undefined())
    {
        msg<<"undefined\n";
        return msg.str();
    }

    for(auto const& [key,value] : *this->type_)
        if(value)
        {
            msg<<"{'"<<tokens::to_string(key)<<"': ";

            // Get data
            t_data=this->data_.at(key);

            // Down cast
            if(!t_data)
                throw std::runtime_error("Invalid token data");

            // Add data
            switch(key)
            {
                case tokens::t_integers:
                    msg<<dynamic_cast<token_data_int*>(t_data)->get_data();
                    break;

                case tokens::t_floats:
                    msg<<dynamic_cast<token_data_float*>(t_data)->get_data();
                    break;

                case tokens::t_identifiers:
                case tokens::t_operators:
                case tokens::t_reserved_words:
                    msg<<dynamic_cast<token_data_string*>(t_data)->get_data();
                    break;

                default:
                    msg<<"undefined";
            }

            msg<<"} ";
        }
    msg <<"\n";

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

// Check the token type
bool token::is_type(tokens::t_type type)
{
    return this->info_->get_type()->at(type);
}

// Check if the token is a reserved word
bool token::is_reserved_word()
{
    return this->is_type(tokens::t_reserved_words);
}

// Check if the token is an assignment
bool token::is_assignment()
{
    auto* t_data= this->get_info()->get_operator_data();

    if(!t_data)
        return false;

    return t_data->get_data()=="=";
}

// Check if the token is a numeric
bool token::is_numeric()
{
    return this->is_type(tokens::t_integers)||this->is_type(tokens::t_floats);
}

// Check if the token is an operator
bool token::is_operator()
{
    return this->is_type(tokens::t_operators);
}

// Check if the token is an identifier
bool token::is_identifier()
{
    return this->is_type(tokens::t_identifiers);
}