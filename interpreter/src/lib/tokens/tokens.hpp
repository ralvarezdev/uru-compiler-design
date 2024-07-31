#pragma once

#include<iostream>
#include<map>
#include<string>

using std::cout;
using std::map;
using std::string;

// Tokens type enum
namespace tokens
{
    enum t_type
    {
        // Higher precedence
        t_reserved_words,
        t_operators,
        t_identifiers,

        // t_digits,
        t_integers,
        t_floats,
        // t_letters,
        // t_words,
        t_end
    };

    // Token type to string
    string to_string(t_type type);
}

// Token data class
class token_data
{
public:
    virtual ~token_data()=default;
};

// Token data class with template
template<class T>
class token_data_t final: public token_data
{
    T data_;

public:
    explicit token_data_t(T data): data_(data){};

    ~token_data_t() override = default;

    // Getters
    T get_data()
    {
        return this->data_;
    }
};

// Token data classes
using token_data_int = token_data_t<int>;
using token_data_float = token_data_t<float>;
using token_data_string = token_data_t<string>;

// Token info class
class token_info final
{
    map<tokens::t_type, token_data*> data_{};
    map<tokens::t_type, bool>* type_{};
    int column_=-1;
    bool is_undefined_=false;

    static map<tokens::t_type, bool>* generate_type(tokens::t_type);
    void cast_key(string&);

public:
    explicit token_info(map<tokens::t_type, bool>* ,  int, string&);
    explicit token_info(const int column, string& key): token_info(nullptr, column, key){};
    explicit token_info(tokens::t_type, int);
    explicit token_info(tokens::t_type, float);
    token_info()=default;

    ~token_info()
    {
        delete type_;

        for(auto const& x : this->data_)
            delete this->get_data(x.first);
    }

    // Getters
    bool is_undefined()
    {
        return this->is_undefined_;
    }

    map<tokens::t_type, bool>* get_type()
    {
        if(this->is_undefined())
            return nullptr;

        return this->type_;
    }

    bool is_type(tokens::t_type type)
    {
        if(this->is_undefined())
            return false;

        return this->type_->at(type);
    }

    int get_column()
    {
        return this->column_;
    }

    // Get data
    token_data* get_data(const tokens::t_type type)
    {
        if(this->is_undefined())
            return nullptr;

        return this->data_.at(type);
    }

    token_data_int* get_int_data()
    {
        if(this->is_undefined()||!this->type_->at(tokens::t_integers))
            return nullptr;

        return dynamic_cast<token_data_int*>(this->data_.at(tokens::t_integers));
    }

    token_data_float* get_float_data()
    {
        if(this->is_undefined()||!this->type_->at(tokens::t_floats))
            return nullptr;

        return dynamic_cast<token_data_float*>(this->data_.at(tokens::t_floats));
    }

    token_data_string* get_identifier_data()
    {
        if(this->is_undefined()||!this->type_->at(tokens::t_identifiers))
            return nullptr;

        return dynamic_cast<token_data_string*>(this->data_.at(tokens::t_identifiers));
    }

    token_data_string* get_reserved_word_data()
    {
        if(this->is_undefined()||!this->type_->at(tokens::t_reserved_words))
            return nullptr;

        return dynamic_cast<token_data_string*>(this->data_.at(tokens::t_reserved_words));
    }

    token_data_string* get_operator_data()
    {
        if(this->is_undefined()||!this->type_->at(tokens::t_operators))
            return nullptr;

        return dynamic_cast<token_data_string*>(this->data_.at(tokens::t_operators));
    }

    string to_string();
};

// Token class
class token final
{
    string key_;
    token_info* info_{};
    map<tokens::t_type, token_data*> data_{};

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
    bool is_type(tokens::t_type);
    bool is_reserved_word();
    bool is_assignment();
    bool is_numeric();
    bool is_operator();
};
