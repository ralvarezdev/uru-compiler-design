#pragma once

#include<string>
#include<vector>

using std::string;
using std::vector;

class validators
{
protected:
    string name_;

public:
    explicit validators(const char*);
    virtual ~validators() = default;

    string get_name();

    virtual bool validate(const string*, bool) =0;
    static vector<char> get_characters(const string*);
};
