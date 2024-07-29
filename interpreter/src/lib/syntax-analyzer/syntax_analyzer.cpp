#include "syntax_analyzer.hpp"

#include<format>
#include<iostream>

#include "../../../../exceptions/expression_exception.hpp"

using std::format;
using std::cout;

// - Syntax analyzer class

// Constructor
syntax_analyzer::syntax_analyzer(const bool debug)
{
    this->debug_ = debug;
}

// Parse line
node* syntax_analyzer::parse_line(const int line_number, deque<token*>* tokens)
{
    node* root = new node();

    // Clone tokens
    auto* node_data = new deque<token*>();
    for (auto token : *tokens)
        node_data->push_back(token);

    // Set node data
    root->set_data(node_data);

    // Parse expression
    parse_expression(line_number,root);

    if(this->debug_)
        cout << root->to_string();

    return root;
}

// Parse expression
void syntax_analyzer::parse_expression(const int line_number,node* root)
{
    token* t;
    deque<token*>* tokens = root->get_data(),* children_tokens, *inner_children_tokens;
    deque<node*> inner_children_nodes,children_nodes;
    int i=static_cast<int>(tokens->size())-1, tokens_start;

    if(tokens->size() == 1)
        return;

    // Intialize children nodes
    children_nodes = deque<node*>();

    // Initialize inner children_nodes nodes
    inner_children_nodes = deque<node*>();

    // Initialize children nodes tokens
    children_tokens = new deque<token*>();

    // Initialize inner children nodes tokens
    inner_children_tokens = new deque<token*>();

    // Check if it's an expression of type "(" + expression + ")"
    if((t=tokens->at(i--))->get_key()!=")")
    {
        // Check if it's an operator
        if(t->get_info()->get_type()->at(tokens::t_operators))
            throw expression_exception(format(
                "Syntax Error: Missing expression at column {} in line {}",
                t->get_info()->get_column(),line_number
                ));

        children_tokens->push_back(t);

        // Set left node
        children_nodes.push_back(new node(children_tokens, nullptr, nullptr));
    }

    else
    {
        children_tokens->push_front(t);

        if(i==0)
            throw expression_exception(format(
                "Syntax Error: Missing expression before ')' at column {} in line {}",
                t->get_info()->get_column(),line_number
                ));

        while(i>0&&(t=tokens->at(i--))->get_key()!="(")
            children_tokens->push_front(t);

        if(t->get_key()!="(")
            throw expression_exception(format(
                "Syntax Error: Missing '(' before ')' at column {} in line {}",
                t->get_info()->get_column(),line_number
                ));

        // Add parentheses
        children_tokens->push_front(t);

        // Check if there's a reserved word
        tokens_start=0;
        if(i>=0&&(t=tokens->at(i))->get_info()->get_type()->at(tokens::t_words))
            if(t->get_key()==reserved_words::print||t->get_key()==reserved_words::root)
            {
                // Set reserved word token
                inner_children_tokens=new deque<token*>();
                inner_children_tokens->push_back(t);
                children_tokens->push_front(t);
                tokens_start=1;
                i--;

                // Set inner node
                inner_children_nodes.push_back(new node(inner_children_tokens, nullptr, nullptr));
            }

        // Set "(" token
        inner_children_tokens = new deque<token*>();
        inner_children_tokens->push_back(children_tokens->at(tokens_start));

        // Set inner node
        inner_children_nodes.push_back(new node(inner_children_tokens, nullptr, nullptr));

        // Set expression token
        inner_children_tokens = new deque<token*>();

        for(int j=tokens_start+1;j<children_tokens->size()-1;j++)
            inner_children_tokens->push_back(children_tokens->at(j));

        // Set inner node
        inner_children_nodes.push_back(new node(inner_children_tokens, nullptr, nullptr));

        // Set ")" token
        inner_children_tokens = new deque<token*>();
        inner_children_tokens->push_back(children_tokens->back());

        // Set inner node
        inner_children_nodes.push_back(new node(inner_children_tokens, nullptr, nullptr));

        // Set siblings
        for(int j=0;j<inner_children_nodes.size()-1;j++)
            inner_children_nodes.at(j)->set_next_sibling(inner_children_nodes.at(j+1));

        // Set last child node
        children_nodes.push_back(new node(children_tokens,inner_children_nodes.front(),nullptr));

        // Parse expression
        parse_expression(line_number, inner_children_nodes.at(tokens_start+1));
    }

    // Check if there are more nodes
    if(i<0)
    {
        // Set first child node
        root->set_first_child(children_nodes.front());
        return;
    }

    // Check if it's an operator
    if(!(t=tokens->at(i--))->get_info()->get_type()->at(tokens::t_operators))
        throw expression_exception(format(
            "Syntax Error: Missing operator before '(' at column {} in line {}",
            t->get_info()->get_column(),line_number
            ));

    // Check if it's another parentheses
    if(t->get_key()=="("||t->get_key()==")")
        throw expression_exception(format(
            "Syntax Error: Missing operator before '(' at column {} in line {}",
            t->get_info()->get_column(),line_number
            ));

    // Set operator child tokens
    inner_children_tokens=new deque<token*>();
    inner_children_tokens->push_back(t);

    // Set operator node
    children_nodes.push_front( new node(inner_children_tokens,nullptr,nullptr));

    // Check if there are more nodes
    if(i<0)
        throw expression_exception(format(
            "Syntax Error: Missing expression before operator at column {} in line {}",
            t->get_info()->get_column(),line_number
            ));

    // Set expression child tokens
    inner_children_tokens = new deque<token*>();
    while(i>=0)
        inner_children_tokens->push_front(tokens->at(i--));

    // Set expression node
    children_nodes.push_front(new node(inner_children_tokens, nullptr, nullptr));

    // Set siblings
    for(int j=0;j<children_nodes.size()-1;j++)
        children_nodes.at(j)->set_next_sibling(children_nodes.at(j+1));

    // Parse expression
    parse_expression(line_number,children_nodes.front());

    // Set left child node
    root->set_first_child(children_nodes.front());
}
