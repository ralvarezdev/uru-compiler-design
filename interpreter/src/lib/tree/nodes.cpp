#include "nodes.hpp"

#include "../tokens/tokens.hpp"

// - Node class

// Constructor
node::node(deque<token*>* data, node* first_child, node* next_sibling)
{
    this->data = data;
    this->first_child = first_child;
    this->next_sibling = next_sibling;
}
