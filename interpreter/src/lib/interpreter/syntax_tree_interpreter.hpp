#pragma once

#include "../tree/syntax_tree_node.hpp"
#include "../lexical-analyzer/lexical_analyzer.hpp"
#include "../tokens/tokens.hpp"

// Parsed number class
class parsed_number
{
    lexical_analyzer* lexical_analyzer_;
    token_info* t_info_;
    int integer_number_;
    float float_number_;

public:
    explicit parsed_number(lexical_analyzer*, token*);
    explicit parsed_number(lexical_analyzer* lexical): parsed_number(lexical,nullptr) {};
    parsed_number(): parsed_number(nullptr) {};

    // Getters
    int get_integer_number()
    {
        return integer_number_;
    }

    float get_float_number()
    {
        return float_number_;
    }
};

// Syntax tree interpreter class
class syntax_tree_interpreter {
    map<string, int> operators_precedence_;
    lexical_analyzer* lexical_analyzer_;
    bool debug_;

    token* calculate_numeric(int,syntax_tree_node*, deque<token*>*, deque<token*>*,deque<token*>*);
    bool has_higher_priority(int, token*, token*);
    void push_operator_to_postfix_stack(int,token*,deque<token*>*, deque<token*>* );
    float calculate_float_op(int,float , float, token*);
    int calculate_integer_op(int,int , int, token*);
    int calculate_integer_op(int,float , float, token*);
    token* operate_numeric_postfix(int,deque<token*>*, deque<token*>*);

public:
    explicit syntax_tree_interpreter(lexical_analyzer*, bool);
    explicit syntax_tree_interpreter(lexical_analyzer* lexical_analyzer) : syntax_tree_interpreter(lexical_analyzer,false) {};
    syntax_tree_interpreter(): syntax_tree_interpreter(nullptr) {};
    ~syntax_tree_interpreter() = default;

    // Getters and setters
    void set_lexical_analyzer(lexical_analyzer* lexical_analyzer)
    {
        this->lexical_analyzer_ = lexical_analyzer;
    }

    lexical_analyzer* get_lexical_analyzer()
    {
        return lexical_analyzer_;
    }

    void interpret(int,syntax_tree_node*);
};
