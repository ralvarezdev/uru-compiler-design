#pragma once

#include<string>

using std::string;

class identifiers {
    public:
        static bool is_letter(int, bool=false);
        static void is_identifier(const string*);
};