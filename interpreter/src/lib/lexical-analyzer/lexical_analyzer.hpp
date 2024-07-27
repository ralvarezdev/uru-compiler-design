#pragma once

#include<string>
#include<map>
#include<list>
#include<iostream>

using std::cout;
using std::string;
using std::map;
using std::list;

#define DEBUG = true

namespace tokens {
    enum token_type {
        DIGITS,
        FLOATS,
        IDENTIFIERS,
        INTEGERS,
        LETTERS,
        OPERATORS,
        WORDS
    };

    class token_info {
    private:
        token_type type;
        string value;

    public:
        token_info(token_type type, string value) : type(type), value(value) {}

        token_type get_type() {
            return this->type;
        }

        string get_value() {
            return this->value;
        }

        string to_string() {
            return "Token type: " + std::to_string(this->type) + " Token value: " + this->value;
        }
    };
}

class lexical_analyzer {
private:
    map<string, tokens::token_info>* symbols_table;

public:
    lexical_analyzer();

    virtual ~lexical_analyzer(){
        this->symbols_table->clear();
        delete this->symbols_table;
    }

    void add_token(const string&, const tokens::token_info&);
    tokens::token_info get_token(const string&);
    void update_token(const string&, const tokens::token_info&);
    void remove_token(const string&);
    void print_tokens();
    string to_string();
};