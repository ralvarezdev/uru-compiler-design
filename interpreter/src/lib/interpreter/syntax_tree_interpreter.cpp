#include "syntax_tree_interpreter.hpp"

#include "../../../../exceptions/expression_exception.hpp"

#include<deque>
#include<format>
#include<iostream>

#include "../../../../dfa-regex-validator/src/lib/validators/reserved_words/reserved_words.hpp"

using std::cout;
using std::format;
using std::deque;

// - Syntax tree interpreter class

// Interpret syntax tree
void syntax_tree_interpreter::interpret(const int line_number,syntax_tree_node* root)
{
    token* current_token;
    token_data* result_numeric;
    syntax_tree_node* current_node=root;

    // Check if the root node contains a print statement
    current_token=current_node->get_data()->front();

    if(current_token->get_key()==r_words::print)
    {
        if(!root->get_first_child())
            throw expression_exception(format(
                "Syntax error: Missing expression after the print statement at column {} in line {}"
                ,current_token->get_info()->get_column(),line_number
                ));

        // Check data type
        current_node=current_node->get_first_child();

        /*
        // Check if it's a numeric expression
        if(current_node->is_numeric())
        {
            result_numeric=calculate_numeric(line_number, current_node->get_first_child());
            if(result_numeric)
                cout<<result_numeric->get_value()<<"\n";
            return;
        }
        */

        // Check expression
        // NOTE: This is still in development. Based on the avalaible data types, the missing data type
        // to be implemented is the string data type
        throw expression_exception(format(
            "NOT IMPLEMENTED: Print statement for the given expression at column {} in line {} is still in development"
            ,current_token->get_info()->get_column(),line_number
            ));
    }
}

// Calculate
token_data*  syntax_tree_interpreter::calculate_numeric(const int line_number,syntax_tree_node* root)
{
    auto tokens = deque<token*>();
    auto children = deque<syntax_tree_node*>();
    token* current_token;
    token_data* result_numeric;
    syntax_tree_node* current_node=root,*main_node, *operator_node, *sibling_node;

    // Get left children
    while((current_node=current_node->get_first_child()))
        children.push_back(current_node);

    // Get outer left node
    current_node=children.back();

    // Get operator
    operator_node=children.back();

    // Get sibling
    sibling_node=current_node->get_next_sibling();

    /*
    // Check if the root node contains an assignment statement
    if(operator_node->get_data()->front()->is_assignment())
    {
        // Check data type
        current_node=current_node->get_first_child();

        // Check if it's a numeric expression
        if(current_node->is_numeric())
        {
            result_numeric=calculate_numeric(line_number, current_node->get_first_child());
            if(result_numeric)
                cout<<result_numeric->get_value()<<"\n";
            return;
        }

        // Check expression
        // NOTE: This is still in development. Based on the avalaible data types, the missing data type
        // to be implemented is the string data type
        throw expression_exception(format(
            "NOT IMPLEMENTED: Let statement for the given expression at column {} in line {} is still in development"
            ,current_token->get_info()->get_column(),line_number
            ));
    }
    */

    return result_numeric;
}

// Calculate
token_data*  syntax_tree_interpreter::operate_numeric(const int line_number,deque<token*>* tokens)
{
    auto children = deque<syntax_tree_node*>();
    token* current_token;
    token_data* result_numeric;

    return result_numeric;
}