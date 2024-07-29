#pragma once

#include<string>

#include"../validators.hpp"

using std::string;

class words final : public validators
{
    static bool status0(int);

public:
    words();
    ~words() override = default;

    bool validate(const string*, bool) override;
    static bool is_word(const string*, bool = false);
};
