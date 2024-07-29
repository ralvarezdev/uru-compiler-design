#pragma once

#include<string>
#include<map>
#include<deque>
#include<iostream>
#include<sstream>

#include"../tokens/tokens.hpp"
#include"../../../../dfa-regex-validator/src/lib/validators/validators.hpp"

using std::stringstream;
using std::cout;
using std::string;
using std::map;
using std::deque;

// Lexical analyzer class
class lexical_analyzer final
{
    map<string, token_info*> symbols_table_;
    map<tokens::t_type,validators*> validators_;
    bool debug_{};

    void add_token(const string&, token_info*);
    void remove_token(const string&);
    map<tokens::t_type, bool>* validate_token(const string&);

public:
    explicit lexical_analyzer(bool);
    lexical_analyzer(): lexical_analyzer(false) {};

    ~lexical_analyzer()
    {
        // Clear symbols table
        for(auto const& x : this->symbols_table_)
            delete x.second;

        this->symbols_table_.clear();

        // Clear validators
        for(auto const& x : this->validators_)
            delete x.second;

        this->validators_.clear();
    }

    token_info* get_token(const string&);
    void update_token(const string&, token_info*);
    void print_tokens();
    deque<token*>* read_line(const string&, int);
};
