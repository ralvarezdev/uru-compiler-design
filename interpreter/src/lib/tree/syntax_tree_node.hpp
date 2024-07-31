#pragma once

#include<deque>
#include<iostream>

#include "../tokens/tokens.hpp"
#include "parse_tree_node.hpp"

using std::deque;
using std::cout;

// - Syntax tree node class
class syntax_tree_node
{
    deque<token*>* data;
    syntax_tree_node *first_child;
    syntax_tree_node *next_sibling;

    static deque<token*>* clone_data(parse_tree_node*);
    string to_string(int);

public:
    explicit syntax_tree_node(deque<token*>*, syntax_tree_node*,syntax_tree_node*);
    explicit syntax_tree_node(deque<token*>* data): syntax_tree_node(data, nullptr, nullptr) {};
    syntax_tree_node(): syntax_tree_node(nullptr) {};

    ~syntax_tree_node()
    {
        // Free memory recursively
        delete next_sibling;;
        delete first_child;
        delete data;
    }

    deque<token*>* get_data()
    {
        return data;
    }

    syntax_tree_node *get_first_child()
    {
        return first_child;
    }

    syntax_tree_node *get_next_sibling()
    {
        return next_sibling;
    }

    void set_data(deque<token*>* data)
    {
        this->data = data;
    }

    void set_first_child(syntax_tree_node* first_child)
    {
        this->first_child = first_child;
    }

    void set_next_sibling(syntax_tree_node* next_sibling)
    {
        this->next_sibling = next_sibling;
    }

    static syntax_tree_node* get_syntax_tree_node(parse_tree_node*);
    string to_string();
    bool is_numeric();
};
