#pragma once

#include<string>

#include "../validators.hpp"

using namespace std;

class digits final : public validators
{
    static bool status0(int);

public:
    digits();
    ~digits() override = default;

    bool validate(const string*, bool) override;
    static bool is_digit(int, bool = false);
    static bool is_digit(const string*, bool = false);
};
