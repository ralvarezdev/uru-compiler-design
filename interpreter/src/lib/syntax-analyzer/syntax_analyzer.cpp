#include "syntax_analyzer.hpp"

#include<format>

#include "../../../../exceptions/expressionException.hpp"

using std::format;

// - Syntax analyzer class

// Constructor
syntax_analyzer::syntax_analyzer(bool debug)
{
    this->debug_ = debug;
}

// Parse line
node* syntax_analyzer::parse_line(const int line_number, deque<token*>* tokens)
{
    node* root = new node();
    token* t;

    // Get token
    t = tokens->back();
    tokens->pop_back();

    // Clone tokens
    deque<token*> node_data = deque<token*>();
    for (auto token : *tokens)
        node_data.push_back(token);

    // Set node data
    root->set_data(&node_data);

    // Parse expression
    parse_expression(line_number,root);

    return root;
}

// Parse expression
void syntax_analyzer::parse_expression(const int line_number,node* root)
{
    token* t;
    deque<token*>* tokens = root->get_data();
    deque<token*>* children_tokens, *inner_children_tokens;
    token_info* t_info;
    deque<node*> inner_children_nodes,children_nodes;
    int i=static_cast<int>(tokens->size()-1),j;

    if(tokens->size() == 1)
        return;

    // Intialize children nodes
    children_nodes = deque<node*>();

    // Initialize inner children_nodes nodes
    inner_children_nodes = deque<node*>();

    // Initialize children_nodes tokens
    children_tokens = new deque<token*>();

    // Check last token
    t=tokens->at(i--);

    // Check if it's an expression of type "(" + expression + ")"
    if(t->get_key()!=")")
    {
        // Check if it's an operator
        if(t->get_info()->get_type()->at(tokens::t_operators))
            throw expression_exception(format(
                "Syntax Error: Missing expression at column {} in line {}",
                t->get_info()->get_column(),line_number
                ));

        inner_children_tokens=new deque<token*>();
        inner_children_tokens->push_back(t);

        children_tokens->push_back(t);

        // Set inner left node
        inner_children_nodes.push_back(new node(children_tokens, nullptr, nullptr));
    }

    else
    {
        // Parse right child
        children_tokens->push_back(t);

        if(i==0)
            throw expression_exception(format(
                "Syntax Error: Missing expression before ')' at column {} in line {}",
                t->get_info()->get_column(),line_number
                ));

        while(i>=0&&(t=tokens->at(i--))->get_key()!="(")
            children_tokens->push_back(t);

        if(t->get_key()!="(")
            throw expression_exception(format(
                "Syntax Error: Missing '(' before ')' at column {} in line {}",
                t->get_info()->get_column(),line_number
                ));

        // Check if there's a reserved word
        if((t=tokens->at(i))->get_info()->get_type()->at(tokens::t_words))
            if(t->get_key()==reserved_words::print||t->get_key()==reserved_words::root)
            {
                // Set reserved word token
                inner_children_tokens=new deque<token*>();
                inner_children_tokens->push_back(t);
                i--;

                // Set inner node
                inner_children_nodes.push_back(new node(inner_children_tokens, nullptr, nullptr));
            }

        j=static_cast<int>(children_tokens->size()-1);

        // Set "(" token
        inner_children_tokens = new deque<token*>();
        inner_children_tokens->push_back(children_tokens->at(j--));

        // Set inner node
        inner_children_nodes.push_back(new node(inner_children_tokens, nullptr, nullptr));

        // Set expression token
        inner_children_tokens = new deque<token*>();
        while(j>1)
            inner_children_tokens->push_back(children_tokens->at(j--));

        // Set inner node
        inner_children_nodes.push_back(new node(inner_children_tokens, nullptr, nullptr));

        // Set ")" token
        inner_children_tokens = new deque<token*>();
        inner_children_tokens->push_back(children_tokens->front());

        // Set inner node
        inner_children_nodes.push_back(new node(inner_children_tokens, nullptr, nullptr));

        // Set siblings
        for(j=0;j<inner_children_nodes.size()-1;j++)
            inner_children_nodes.at(j)->set_next_sibling(inner_children_nodes.at(j+1));

        // Parse expression
        parse_expression(line_number, inner_children_nodes.at(inner_children_nodes.size()-2));
    }

    // Set last child node
    children_nodes.push_back(new node(inner_children_tokens,inner_children_nodes.front(),nullptr));

    // Check if there are more nodes
    if(i==0)
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
    children_nodes.push_front( new node(inner_children_tokens,inner_children_nodes.front(),nullptr));

    // Check if there are more nodes
    if(i==0)
        throw expression_exception(format(
            "Syntax Error: Missing expression before operator at column {} in line {}",
            t->get_info()->get_column(),line_number
            ));

    // Set expression child tokens
    inner_children_tokens = new deque<token*>();
    while(i>=0)
        inner_children_tokens->push_back(tokens->at(i--));

    // Set expression node
    children_nodes.push_front(new node(children_tokens, inner_children_nodes.front(), nullptr));

    // Set siblings
    for(j=0;j<children_nodes.size()-1;j++)
        children_nodes.at(j)->set_next_sibling(children_nodes.at(j+1));

    // Parse expression
    parse_expression(line_number,children_nodes.front());

    // Set left child node
    root->set_first_child(children_nodes.front());
}
