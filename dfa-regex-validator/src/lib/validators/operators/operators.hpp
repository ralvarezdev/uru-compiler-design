#pragma once

#include<string>

using std::string;

class operators
{
public:
    static bool is_operator(int, bool = true);
    static void is_operator(const string*);
};
