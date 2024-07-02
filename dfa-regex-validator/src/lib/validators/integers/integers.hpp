#pragma once

#include<string>

#include "../validators.hpp"

using std::string;

class integers final:public validators{
    static bool status0(int);

    public:
        integers();
        ~integers() override=default;

        bool validate(const string*, bool) override;
        static bool is_integer(const string*, bool=false);
};