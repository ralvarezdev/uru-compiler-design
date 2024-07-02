#pragma once

#include<string>

#include "../validators.hpp"

using std::string;

class identifiers final:public validators{
    static bool is_extended_letter(int);
    static bool status0(int);
    static bool status1(int);

    public:
        identifiers();
        ~identifiers() override=default;

    bool validate(const string*, bool) override;
    static bool is_identifier(const string*, bool=false);
};