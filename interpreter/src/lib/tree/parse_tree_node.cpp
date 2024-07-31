#include "parse_tree_node.hpp"

#include<sstream>
#include<iostream>

#include "../tokens/tokens.hpp"

using std::stringstream;
using std::cout;

// - Node class

// Constructor
parse_tree_node::parse_tree_node(deque<token*>* data, parse_tree_node* first_child, parse_tree_node* next_sibling)
{
    this->data = data;
    this->first_child = first_child;
    this->next_sibling = next_sibling;
}

// To string
string parse_tree_node::to_string(const int level)
{
    const auto tab = string(level, '\t');

    stringstream msg;
    msg<<'\n'<<tab<<"Data: ";
    for(auto const& t : *this->data)
        msg<<"'"<<t->get_key()<<"' ";
    msg<<"\n";

    // Print children
    if(this->first_child)
        msg<<tab<<"First child: {"<<this->first_child->to_string(level+1)<<tab<<"}\n";

    // Print sibling
    if(this->next_sibling)
    {
        msg<<tab<<"- Next sibling"<<this->next_sibling->to_string(level);

        if(!this->next_sibling->get_next_sibling())
            msg<<tab<<"}\n";
    }

    return msg.str();
}

string parse_tree_node::to_string()
{
    return this->to_string(0);
}
