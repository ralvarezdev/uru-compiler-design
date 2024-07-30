#pragma once

#include<deque>
#include<iostream>

#include "../tokens/tokens.hpp"

using std::deque;
using std::cout;

// - Parse tree node class
class parse_tree_node
{
    deque<token*>* data;
    parse_tree_node *first_child;
    parse_tree_node *next_sibling;

    string to_string(int);

public:
    explicit parse_tree_node(deque<token*>*, parse_tree_node*, parse_tree_node*);
    explicit parse_tree_node(deque<token*>* data): parse_tree_node(data, nullptr, nullptr) {};
    parse_tree_node(): parse_tree_node(nullptr) {};

    ~parse_tree_node()
    {
        // Free memory recursively
        delete next_sibling;;
        delete first_child;

        /*
        // Debug
        cout<<"Deleting data: ";
        for(auto const& t : *data)
            cout<<"'"<<t->get_key()<<"' ";
        cout<<"\n";
        */

        delete data;
    }

    deque<token*>* get_data()
    {
        return data;
    }

    parse_tree_node *get_first_child()
    {
        return first_child;
    }

    parse_tree_node *get_next_sibling()
    {
        return next_sibling;
    }

    void set_data(deque<token*>* data)
    {
        this->data = data;
    }

    void set_first_child(parse_tree_node* first_child)
    {
        this->first_child = first_child;
    }

    void set_next_sibling(parse_tree_node* next_sibling)
    {
        this->next_sibling = next_sibling;
    }

    string to_string();
};
