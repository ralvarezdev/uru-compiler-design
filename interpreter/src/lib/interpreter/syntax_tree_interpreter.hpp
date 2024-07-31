#pragma once

#include "../tree/syntax_tree_node.hpp"
#include "../lexical-analyzer/lexical_analyzer.hpp"
#include "../tokens/tokens.hpp"

// Syntax tree interpreter class
class syntax_tree_interpreter {
    lexical_analyzer* lexical_analyzer_;
    bool debug_;

    token_data* calculate_numeric(int,syntax_tree_node*);
    token_data* operate_numeric(int, deque<token*>*);

public:
    explicit syntax_tree_interpreter(lexical_analyzer* lexical_analyzer, const bool debug) : lexical_analyzer_(lexical_analyzer), debug_(debug) {}
    explicit syntax_tree_interpreter(lexical_analyzer* lexical_analyzer) : syntax_tree_interpreter(lexical_analyzer,false) {};
    syntax_tree_interpreter(): syntax_tree_interpreter(nullptr) {};

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
