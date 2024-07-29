#pragma once

#include<map>
#include<string>

using std::map;
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

// Reserved words
namespace  reserved_words
{
    const string print = "print";
    const string root = "root";
}

// Token info class
class token_info final
{
    map<tokens::t_type, bool>* type_{};
    int column_{};

public:
    explicit token_info(map<tokens::t_type, bool>* type, int);
    token_info()=default;

    ~token_info()
    {
        type_->clear();
        delete type_;
    }

    // Getters
    map<tokens::t_type, bool>* get_type()
    {
        return this->type_;
    }

    int get_column()
    {
        return this->column_;
    }

    string to_string();
};

// Token class
class token final
{
    string key_;
    token_info* info_{};

public:
    explicit token(const string&, token_info*);
    token()= default;

    // Getters
    string get_key()
    {
        return this->key_;
    }

    token_info* get_info()
    {
        return this->info_;
    }

    string to_string();
};
