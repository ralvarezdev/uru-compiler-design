#pragma once

#include<string>

#include "../validators.hpp"

using std::string;

class letters final : public validators
{
     static bool status0(int);

public:
     letters();
     ~letters() override = default;

     bool validate(const string*, bool) override;
     static bool is_letter(const string*, bool = false);
     static bool is_letter(int, bool = false);
};
