#pragma once

#include<string>

#include "../validators.hpp"

using std::string;

class floats final:public validators
{
    static bool status0(int);
    static bool status1(int);
    static bool status2(int);

public:
    floats();
    ~floats() override=default;

    bool validate(const string*, bool) override;
    static bool is_float(const string*, bool);
};
