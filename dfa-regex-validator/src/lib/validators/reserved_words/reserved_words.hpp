#pragma once

#include<string>
#include<deque>

#include "../validators.hpp"

using std::deque;

using namespace std;

// Reserved words
namespace  r_words
{
    const string print = "print";
    const string root = "root";

    const deque<string> reserved_words = {print, root};
}

class reserved_words final : public validators
{
public:
    reserved_words();
    ~reserved_words() override = default;

    bool validate(const string*, bool) override;
    static bool is_reserved_word(const string*, bool = false);
};
