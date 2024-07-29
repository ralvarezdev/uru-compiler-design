#pragma once

#include<deque>

#include "../tokens/tokens.hpp"
#include "../tree/nodes.hpp"

using std::deque;

class syntax_analyzer
{
    bool debug_;

public:
    explicit syntax_analyzer(bool);
    syntax_analyzer(): syntax_analyzer(false) {};

    node* parse_line(int,deque<token*>*);
    void parse_expression(int,node*);
};
