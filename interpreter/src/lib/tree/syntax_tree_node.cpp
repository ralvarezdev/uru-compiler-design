#include "syntax_tree_node.hpp"

#include<deque>
#include<sstream>

#include "parse_tree_node.hpp"

using std::deque;
using std::string;
using std::stringstream;

// - Syntax tree parse node class

// Constructor
syntax_tree_node::syntax_tree_node(deque<token*>* data, syntax_tree_node* first_child, syntax_tree_node* next_sibling)
{
    this->data = data;
    this->first_child = first_child;
    this->next_sibling = next_sibling;
}

// Clone data
deque<token*>* syntax_tree_node::clone_data(parse_tree_node* parse_node)
{
    auto* new_data = new deque<token*>;

    for(auto t : *parse_node->get_data())
        new_data->push_back(t);

    return new_data;
}

syntax_tree_node* syntax_tree_node::get_syntax_tree_node(parse_tree_node* parse_node)
{
    parse_tree_node* first_child, *old_last_child,*last_child, *expression_child;
    deque<syntax_tree_node*> children;
    deque<token*>* new_data, *child_data;
    token* first_token;

    if(parse_node == nullptr)
        return nullptr;

    // Get first child
    first_child=parse_node->get_first_child();

    // If parse node has no children
    if(!first_child)
        return new syntax_tree_node(clone_data(parse_node));

    // Create new data
    new_data = new deque<token*>();

    // Get child data
    child_data = first_child->get_data();
    first_token = child_data->front();

    // If first child is a reserved word
    if(first_token->get_info()->get_type()->at(tokens::t_reserved_words))
        {
            // Add reserved word token
            new_data->push_back(first_token);

            // Get inner child with the expression
            expression_child=first_child->get_next_sibling()->get_next_sibling();

            // Create new syntax node
            return new syntax_tree_node(new_data, get_syntax_tree_node(expression_child), nullptr);
        }

    // Add "(" token, if needed
    const bool is_inside_parenthesis=first_token->get_key() == "(";

    if(is_inside_parenthesis)
    {
        new_data->push_back(first_token);
        old_last_child=first_child->get_next_sibling()->get_next_sibling();
        first_child=first_child->get_next_sibling()->get_first_child();
    }

    // Get expression and "last" child
    expression_child=first_child->get_next_sibling();
    last_child=expression_child->get_next_sibling();
    child_data = expression_child->get_data();

    // Add expression token
    new_data->push_back(child_data->front());

    // Add ")" token, if needed
    if(is_inside_parenthesis)
        new_data->push_back(old_last_child->get_data()->front());

    // Set new children
    children=deque<syntax_tree_node*>{get_syntax_tree_node(first_child), get_syntax_tree_node(last_child)};

    // Set new siblings
    for(int i=0;i<children.size()-1;i++)
        children.at(i)->next_sibling = children.at(i+1);

    // Create new syntax node
    return new syntax_tree_node{new_data, children.front(), nullptr};
}

// Get syntax tree parse node from parse tree parse node

// To string
string syntax_tree_node::to_string(int level)
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

string syntax_tree_node::to_string()
{
    return this->to_string(0);
}

// Check if the syntax tree node is all numeric
bool syntax_tree_node::is_numeric()
{
    // Check first child
    if(this->get_first_child())
    {
        if(!this->get_first_child()->is_numeric())
            return false;
    }
    else
        for(auto const& t : *this->data)
        {
            if(t->is_operator())
                continue;

            if(!t->is_numeric())
                return false;
        }

    // Check sibling
    if(this->get_next_sibling())
        if(!this->get_next_sibling()->is_numeric())
            return false;

    return true;
}
