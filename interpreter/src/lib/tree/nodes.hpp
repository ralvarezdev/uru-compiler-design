#pragma once

#include<deque>
#include<iostream>

#include "../tokens/tokens.hpp"

using std::deque;
using std::cout;

class node
{
    deque<token*>* data;
    node *first_child;
    node *next_sibling;

public:
    explicit node(deque<token*>*, node*, node*);
    node(): node(nullptr, nullptr, nullptr) {};

    ~node()
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

    node *get_first_child()
    {
        return first_child;
    }

    node *get_next_sibling()
    {
        return next_sibling;
    }

    void set_data(deque<token*>* data)
    {
        this->data = data;
    }

    void set_first_child(node* first_child)
    {
        this->first_child = first_child;
    }

    void set_next_sibling(node* next_sibling)
    {
        this->next_sibling = next_sibling;
    }

    string to_string(int);
    string to_string();
};
