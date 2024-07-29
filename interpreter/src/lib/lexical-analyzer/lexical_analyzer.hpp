#pragma once

#include<string>
#include<map>
#include<vector>
#include<iostream>
#include<sstream>

#include"../tokens/tokens.hpp"
#include"../../../../dfa-regex-validator/src/lib/validators/validators.hpp"

using std::stringstream;
using std::cout;
using std::string;
using std::map;
using std::vector;

// Reserved words
namespace  reserved_words
{
    const string print = "print";
    const string root = "root";
}

// Token info class
class token_info final
{
    map<tokens::t_type, bool>* type_;

public:
    explicit token_info(map<tokens::t_type, bool>* type);
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

    string to_string()
    {
        stringstream msg;

        msg<<"Token types: ";
        for(auto const& [key,value] : *this->type_)
        {
            if(value)
                msg<<tokens::to_string(key)<<" ";
        }

        return msg.str();
    }
};

// Token class
class token final
{
    string key_;
    token_info* info_{};

public:
    explicit token(const string&, token_info*);
    token()= default;

    ~token()
    {
        delete this->info_;
    }

    // Getters
    string get_key()
    {
        return this->key_;
    }

    token_info* get_info()
    {
        return this->info_;
    }

    string to_string()
    {
        stringstream msg;

        msg<<"Token name: "<<this->key_<<"\n";
        msg<<this->info_->to_string();

        return msg.str();
    }
};

// Lexical analyzer class
class lexical_analyzer final
{
    map<string, token_info> symbols_table_;
    map<tokens::t_type,validators*> validators_;
    bool debug_{};

    void add_token(const string&, const token_info&);
    void remove_token(const string&);
    map<tokens::t_type, bool>* validate_token(const string&);

public:
    explicit lexical_analyzer(bool);
    lexical_analyzer(): lexical_analyzer(false) {};

    ~lexical_analyzer()
    {
        // Clear symbols table
        this->symbols_table_.clear();

        // Clear validators
        for(auto const& x : this->validators_)
            delete x.second;

        this->validators_.clear();
    }

    token_info get_token(const string&);
    void update_token(const string&, const token_info&);
    void print_tokens();
    vector<token*>* read_line(const string&, int);
};
