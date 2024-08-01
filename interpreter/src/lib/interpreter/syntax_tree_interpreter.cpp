#include "syntax_tree_interpreter.hpp"

#include "../../../../exceptions/expression_exception.hpp"

#include<deque>
#include<format>
#include<iostream>
#include<cmath>

#include "../../../../dfa-regex-validator/src/lib/validators/reserved_words/reserved_words.hpp"

using std::pow;
using std::cout;
using std::format;
using std::deque;

// - Parsed number clas

// Constructor
parsed_number::parsed_number(lexical_analyzer* lexical,token* t)
{
    this->lexical_analyzer_=lexical;

    // Check if it's an identifier
    if(!t->get_info()->is_type(tokens::t_identifiers))
        this->t_info_=t->get_info();
    else
        this->t_info_=this->lexical_analyzer_->get_token_info(t->get_key());

    if(this->t_info_->is_type(tokens::t_integers))
    {
        this->integer_number_=this->t_info_->get_int_data()->get_data();
        this->float_number_=static_cast<float>(this->integer_number_);
    }
    else if(this->t_info_->is_type(tokens::t_floats))
    {
        this->float_number_=this->t_info_->get_float_data()->get_data();
        this->integer_number_=static_cast<int>(this->float_number_);
    }
}

// - Syntax tree interpreter class

// Constructor
syntax_tree_interpreter::syntax_tree_interpreter(lexical_analyzer* lexical_analyzer, bool debug)
{
    // Set lexical analyzer
    this->lexical_analyzer_=lexical_analyzer;

    // Set debug mode
    this->debug_=debug;

    // Initialize operators precedence
    // Operator precedence
    this->operators_precedence_= map<string,int>{
            {")",0}, {"+", 1}, {"-", 1}, {"*", 2}, {"/", 2}, {"^", 3}, {"(", 4}
    };
}

// Interpret syntax tree
void syntax_tree_interpreter::interpret(const int line_number,syntax_tree_node* root)
{
    deque<token*>* operators,*postfix_operators, *numeric_tokens;
    token* current_token, *result_token;
    token_info* result_info;
    syntax_tree_node* current_node=root;

    // Check if the root node contains a print statement
    current_token=current_node->get_data()->front();

    // Initialize tokens containers
    operators=new deque<token*>();
    postfix_operators=new deque<token*>();
    numeric_tokens=new deque<token*>();

    if(current_token->get_key()==r_words::print)
    {
        if(!root->get_first_child())
            throw expression_exception(format(
                "Runtime error: Missing expression after the print statement at column {} in line {}"
                ,current_token->get_info()->get_column(),line_number
                ));

        // Check data type
        current_node=current_node->get_first_child();

        // Check if it's a numeric expression
        if(current_node->is_numeric())
        {
            result_token=calculate_numeric(line_number, current_node->get_first_child(), operators,postfix_operators, numeric_tokens);

            // Get result token info
            result_info=result_token->get_info();

            // Check if it's an integer
            if(result_info->is_type(tokens::t_integers))
                cout<<result_info->get_int_data();

            // Check if it's a float
            else if(result_info->is_type(tokens::t_floats))
                cout<<result_info->get_float_data();

            // Invalid data type
            else
                throw expression_exception(format(
                    "Runtime error: Result data type is not numeric for the given expression at column {} in line {}"
                    ,current_token->get_info()->get_column(),line_number
                    ));

            cout <<"\n";
            return;
        }

        // Check expression
        // NOTE: This is still in development. Based on the avalaible data types, the missing data type
        // to be implemented is the string data type
        throw expression_exception(format(
            "NOT IMPLEMENTED: Print statement for the given expression at column {} in line {} is still in development"
            ,current_token->get_info()->get_column(),line_number
            ));
    }

    // Calculate numeric expression
    if(current_node->is_numeric())
    {
        calculate_numeric(line_number, current_node->get_first_child(), operators,postfix_operators, numeric_tokens);
        return;
    }

    // Check expression
    // NOTE: This is still in development. Based on the avalaible data types, the missing data type
    // to be implemented is the string data type
    throw expression_exception(format(
        "NOT IMPLEMENTED: Calculate function for the given expression at column {} in line {} is still in development"
        ,current_token->get_info()->get_column(),line_number
        ));
}

// Calculate numeric tokens
token*  syntax_tree_interpreter::calculate_numeric(const int line_number,syntax_tree_node* root, deque<token*>* operators,deque<token*>* postfix_operators, deque<token*>* numeric_tokens)
{
    deque<token*> *new_tokens,*new_operators,*new_numeric_tokens,*new_postfix_operators;
    auto children = deque<syntax_tree_node*>();
    token* t,*identifier, *main_operator=nullptr;
    syntax_tree_node* current_node=root,*first_child,*operator_node, *sibling_node,*new_sibling_node;

    // Get children
    while((current_node=current_node->get_first_child()))
        children.push_back(current_node);

    while(!children.empty())
    {
        // Get outer left node
        current_node=children.back();
        children.pop_back();
        numeric_tokens->push_back(current_node->get_data()->front());

        // Get operator
        operator_node=children.back();
        children.pop_back();
        for(auto& t: *operator_node->get_data())
        {
            if(!main_operator)
            {
                // Check if it's a parentheses
                if(t->get_key()!="("&&t->get_key()!=")")
                    main_operator=t;
            }

            // Check if it's an assignment operator
            if(!t->is_assignment())
                this->push_operator_to_postfix_stack(line_number, t, postfix_operators, operators);
        }

        // Get sibling
        sibling_node=current_node->get_next_sibling();

        if((first_child=sibling_node->get_first_child()))
        {
            // Initialize tokens containers
            new_numeric_tokens= new deque<token*>();
            new_operators=new deque<token*>();
            new_postfix_operators=new deque<token*>();

            // Calculate result of the given
            t=calculate_numeric(line_number, first_child, new_operators, new_postfix_operators, new_numeric_tokens);

            // Add numeric token
            numeric_tokens->push_back(t);
            delete sibling_node;
        }
    }

    // Empty operators
    while(!operators->empty()){
        postfix_operators->push_back(operators->back());
        operators->pop_back();
    }

    // Free memory
    delete operators;

    // Check if the main operator is an assignment
    if(main_operator->is_assignment())
    {
        // Get token to be assigned
        identifier=numeric_tokens->front();
        numeric_tokens->pop_front();

        // Get result
        t=operate_numeric_postfix(line_number, postfix_operators, numeric_tokens);

        // Update identifier
        this->lexical_analyzer_->update_token(identifier->get_key(), t->get_info());
        return t;
    }

    // Check if the main operator is a reserved word
    if(main_operator->is_reserved_word())
    {
        // Check if it's a print statement
        if(main_operator->get_key()==r_words::print)
            throw expression_exception(format(
                "Runtime error: Print statement found inside another expression at column {} in line {}"
                ,main_operator->get_info()->get_column(),line_number
                ));

        // Check if it's a root statement
        if(main_operator->get_key()==r_words::root)
        {
            // Get result
            t=operate_numeric_postfix(line_number, postfix_operators, numeric_tokens);

            // Check if it's an integer
            if(t->get_info()->is_type(tokens::t_integers))
            {
                int data = static_cast<int>(sqrt(t->get_info()->get_int_data()->get_data()));

                // Free memory
                delete t;

                return new token("", new token_info(tokens::t_integers, data));
            }

            // Check if it's a float
            if(t->get_info()->is_type(tokens::t_floats))
            {
                float data = sqrt(t->get_info()->get_float_data()->get_data());

                // Free memory
                delete t;

                return new token("", new token_info(tokens::t_floats,data));
            }

            // Invalid data type
            throw expression_exception(format(
                "Runtime error: Result data type is not numeric for the given expression at column {} in line {}"
                ,main_operator->get_info()->get_column(),line_number
                ));
        }
    }

        // Get result
        return operate_numeric_postfix(line_number, postfix_operators, numeric_tokens);
}

// Check if the operator has higher priority
bool syntax_tree_interpreter::has_higher_priority(const int line_number, token* op1, token* op2)
{
    // Check if the operator is defined
    for(auto op:deque{op1,op2})
        if(!this->operators_precedence_.contains(op->get_key()))
            throw expression_exception(
                format("Runtime error: Invalid operator '{}' found at column {} in line {}",
                op->get_key(), op->get_info()->get_column(), line_number));

    return this->operators_precedence_[op1->get_key()] > this->operators_precedence_[op2->get_key()];
}

// Get operator stack
void syntax_tree_interpreter::push_operator_to_postfix_stack(const int line_number,token*t_op,deque<token*>* postfix_operators, deque<token*>* temp)
{
    // Check top operator
    if(t_op->get_key()==")")
    {
        while(!temp->empty()&&temp->back()->get_key()!="(")
        {
            postfix_operators->push_back(temp->back());
            temp->pop_back();
        }

        if(temp->empty())
            throw expression_exception(
                format("Runtime error: Invalid operation at column {} in line {}",
                t_op->get_info()->get_column(), line_number));

        temp->pop_back();
        return;
    }

    if(temp->empty()||this->has_higher_priority(line_number, t_op, temp->back()))
    {
        temp->push_back(t_op);
        return;
    }

    while(!temp->empty()&&!this->has_higher_priority(line_number, t_op, temp->back())&&temp->back()->get_key()!="(")
    {
        postfix_operators->push_back(temp->back());
        temp->pop_back();
    }

    temp->push_back(t_op);
}

// Operate numeric postfix stack
token* syntax_tree_interpreter::operate_numeric_postfix(const int line_number,deque<token*>* postfix_operators, deque<token*>*numeric_tokens)
{
    auto temp_tokens = deque<token*>();
    token* t1, *t2, *t_op, *result;
    parsed_number p1,p2;
    int integer_result;
    float float_result;

    // Check if the postfix stack is empty
    if(postfix_operators->empty())
        throw expression_exception(
            format("Runtime error: Empty postfix stack at line {}", line_number));

    // Check if there's only one element
    if(postfix_operators->size()==1)
        return postfix_operators->front();

    while(!postfix_operators->empty())
    {
        t_op=postfix_operators->front();
        postfix_operators->pop_front();

        if(numeric_tokens->size()<2)
            throw expression_exception(
                format("Runtime error: Invalid operation at column {} in line {}",
                t_op->get_info()->get_column(), line_number));

        // Get top tokens
        t2=numeric_tokens->back();
        numeric_tokens->pop_back();
        t1=numeric_tokens->back();
        numeric_tokens->pop_back();

        // Get parsed numbers
        p2= parsed_number(this->lexical_analyzer_,t2);
        p1= parsed_number(this->lexical_analyzer_,t1);

        if(t1->is_type(tokens::t_floats)||t2->is_type(tokens::t_floats))
        {
            float_result=this->calculate_float_op(line_number, p1.get_float_number(),p2.get_float_number(), t_op);

            // Create temporary token
            result = new token("", new token_info(tokens::t_floats, float_result));
        }
        else
        {
            integer_result=this->calculate_integer_op(line_number, p1.get_integer_number(),p2.get_integer_number(), t_op);

            // Create temporary token
            result = new token("", new token_info(tokens::t_integers, integer_result));
        }

        // Add result token
        numeric_tokens->push_back(result);
        temp_tokens.push_back(result);
    }

    // Get final result token
    result=numeric_tokens->front();

    // Deallocate temporary tokens
    for(auto& t:temp_tokens)
        if(t!=result)
        delete t;

    // Free memory
    delete numeric_tokens;
    delete postfix_operators;

    return result;
}

// Calculate operation between two float tokens
float syntax_tree_interpreter::calculate_float_op(const int line_number, const float f1, const float f2, token* t_op) {
    string op = t_op->get_key();

    if (op == "+")
        return f1 + f2;
    if (op == "-")
        return f1 - f2;
    if (op == "*")
        return f1 * f2;
    if (op == "/")
        return f1 / f2;
    if (op == "^")
        return pow(f1, f2);

    throw expression_exception(
        format("Runtime error: Invalid operator '{}' found at column {} in line {}",
        op, t_op->get_info()->get_column(), line_number));
}

// Calculate operation between two integer tokens
int syntax_tree_interpreter::calculate_integer_op(const int line_number,const int n1,const int n2, token* t_op) {
    // Cast to float
    auto n1_casted = static_cast<float>(n1);
    auto n2_casted = static_cast<float>(n2);

    return calculate_integer_op(line_number, n1_casted, n2_casted, t_op);
}

int syntax_tree_interpreter::calculate_integer_op(const int line_number,const float n1,const float n2, token* t_op) {
    return static_cast<int>(calculate_float_op(line_number, n1,n2, t_op));
}
