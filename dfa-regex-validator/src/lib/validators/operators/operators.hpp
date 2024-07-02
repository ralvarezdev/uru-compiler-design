#pragma once

#include<string>

#include "../validators.hpp"

using std::string;

class operators final: public validators
{
    static bool status0(int, const int*);

public:
    operators();
    ~operators() override =default;

    bool validate(const string*, bool) override;
    static bool is_operator(const string*, bool = false);
    static bool is_operator(int, bool = false);
};
