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

    if(!t)
        return;

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
    deque<token*>* operators,*postfix;
    token* current_token, *result_token;
    token_info* result_info;
    syntax_tree_node* current_node=root;

    // Check if the root node contains a print statement
    current_token=current_node->get_data()->front();

    // Initialize tokens containers
    operators=new deque<token*>();
    postfix=new deque<token*>();

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
            result_token=calculate_numeric(line_number, current_node, operators,postfix);

            // Get result token info
            result_info=result_token->get_info();

            // Check if it's an integer
            if(result_info->is_type(tokens::t_integers))
                cout<<result_info->get_int_data()->get_data();

            // Check if it's a float
            else if(result_info->is_type(tokens::t_floats))
                cout<<result_info->get_float_data()->get_data();

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
            ,current_token->get_info()->get_column(),line_number));
    }

    // Calculate numeric expression
    if(current_node->is_numeric())
    {
        calculate_numeric(line_number, current_node, operators,postfix);
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
token*  syntax_tree_interpreter::calculate_numeric(const int line_number,syntax_tree_node* root, deque<token*>* operators,deque<token*>* postfix)
{
    deque<token*> *tokens,*new_operators,*new_postfix_operators;
    auto children = deque<syntax_tree_node*>();
    token* t,*identifier, *main_operator=nullptr,*reserved_word=nullptr;
    syntax_tree_node* current_node=root, *sibling_node,*temp_node;

    if(this->debug_)
        cout<<"Root node: "<<root->to_string()<<'\n';

    // Check if the root node has children
    if(!root->get_first_child())
    {
        t=root->get_data()->front();

        // Check if it's a identifier
        if(t->is_identifier())
            return new token("", this->lexical_analyzer_->get_token_info(t->get_key()));
        return root->get_data()->front();
    }

    // Add root operator
    children.push_back((current_node=root));

    // Get children
    while((current_node=current_node->get_first_child()))
        children.push_back(current_node);

    // Get outer left node
    current_node=children.back();
    children.pop_back();
    postfix->push_back(current_node->get_data()->front());

    if(this->debug_)
        cout<<"Current node: "<<current_node->to_string()<<'\n';

    while(!children.empty())
    {
        temp_node=current_node;

        // Get operator
        if(!children.empty())
        {
            current_node=children.back();
            children.pop_back();

            for(auto& t: *current_node->get_data())
            {
                if(!main_operator)
                {
                    // Check if it's a parentheses
                    if(t->get_key()!="("&&t->get_key()!=")")
                        main_operator=t;
                }

                // Check if it's an assignment operator
                if(!t->is_assignment())
                    this->push_operator_to_postfix_stack(line_number, t, postfix, operators);
            }

            if(this->debug_)
                cout<<"Operator: "<<current_node->to_string()<<'\n';
        }

        // Get sibling
        sibling_node=temp_node->get_next_sibling();

        // Check if it has a next sibling
        if(!sibling_node)
            break;

        if(!sibling_node->get_first_child())
            t=sibling_node->get_data()->front();

        else
        {
            // Initialize tokens containers
            new_operators=new deque<token*>();
            new_postfix_operators=new deque<token*>();

            // Get tokens
            tokens=sibling_node->get_data();

            // Remove additional tokens
            tokens->pop_back();

            if(tokens->front()->get_key()!="(")
            {
                reserved_word=tokens->front();
                tokens->pop_front();
            }
            tokens->pop_front();

            // Calculate result of the given
            syntax_tree_node* inner_node=reserved_word?sibling_node->get_first_child():sibling_node;
            t=calculate_numeric(line_number, inner_node, new_operators, new_postfix_operators);

            if(this->debug_)
                cout<<"Inner node final token: "<<t->to_string()<<'\n';

            // Check if there's a reserved word
            if(reserved_word)
            {
                // Check if it's a print statement
                if(reserved_word->get_key()==r_words::print)
                    throw expression_exception(format(
                        "Runtime error: Print statement found inside another expression at column {} in line {}"
                        ,reserved_word->get_info()->get_column(),line_number
                        ));

                // Check if it's a root statement
                if(reserved_word->get_key()==r_words::root)
                {
                    // Check if it's an integer
                    if(t->get_info()->is_type(tokens::t_integers))
                    {
                        int data = static_cast<int>(sqrt(t->get_info()->get_int_data()->get_data()));

                        // Free memory
                        delete t;

                        t= new token("", new token_info(tokens::t_integers, data));
                    }

                    // Check if it's a float
                    else if(t->get_info()->is_type(tokens::t_floats))
                    {
                        float data = sqrt(t->get_info()->get_float_data()->get_data());

                        // Free memory
                        delete t;

                        t= new token("", new token_info(tokens::t_floats,data));
                    }

                    else
                        // Invalid data type
                    throw expression_exception(format(
                        "Runtime error: Result data type is not numeric for the given expression at column {} in line {}"
                        ,reserved_word->get_info()->get_column(),line_number
                        ));
                }
            }
            reserved_word=nullptr;
        }

        // Add numeric token
        postfix->push_back(t);

        if(this->debug_)
            cout<<"Sibling token node: "<<t->to_string()<<'\n';
    }

    // Empty operators
    while(!operators->empty()){
        postfix->push_back(operators->back());
        operators->pop_back();
    }

    // Free memory
    delete operators;

    // Check if the main operator is an assignment
    if(main_operator->is_assignment())
    {
        // Get token to be assigned
        identifier=postfix->front();
        postfix->pop_front();

        if(this->debug_)
            cout<<"Identifier: "<<identifier->get_key()<<'\n';

        // Get result
        t=operate_numeric_postfix(line_number, postfix);

        // Update identifier
        this->lexical_analyzer_->update_token(identifier->get_key(), t->get_info());
        return t;
    }

    // Get result
    return operate_numeric_postfix(line_number, postfix);
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
void syntax_tree_interpreter::push_operator_to_postfix_stack(const int line_number,token*t_op,deque<token*>* postfix, deque<token*>* operators)
{
    if(this->debug_)
        cout<<"Operator: "<<t_op->to_string()<<'\n';

    // Check top operator
    if(t_op->get_key()==")")
    {
        while(!operators->empty()&&operators->back()->get_key()!="(")
        {
            postfix->push_back(operators->back());
            operators->pop_back();
        }

        if(operators->empty())
            throw expression_exception(
                format("Runtime error: Invalid operation at column {} in line {}",
                t_op->get_info()->get_column(), line_number));

        operators->pop_back();
        return;
    }

    if(operators->empty()||this->has_higher_priority(line_number, t_op, operators->back()))
    {
        operators->push_back(t_op);
        return;
    }

    while(!operators->empty()&&!this->has_higher_priority(line_number, t_op, operators->back())&&operators->back()->get_key()!="(")
    {
        postfix->push_back(operators->back());
        operators->pop_back();
    }

    operators->push_back(t_op);
}

// Operate numeric postfix stack
token* syntax_tree_interpreter::operate_numeric_postfix(const int line_number,deque<token*>* postfix)
{
    deque<token*> temp_tokens, numeric_stack;
    token* t1, *t2, *t_op, *result;
    parsed_number p1,p2;
    int integer_result;
    float float_result;

    // Check if the postfix stack is empty
    if(postfix->empty())
        throw expression_exception(
            format("Runtime error: Empty postfix stack at line {}", line_number));

    // Check if there's only one element
    if(postfix->size()==1)
        return postfix->front();

    while(!postfix->empty())
    {
        t_op=postfix->front();
        postfix->pop_front();

        if(!t_op->is_operator())
        {
            numeric_stack.push_back(t_op);
            if(this->debug_)
                cout<<"Pushed: "<<t_op->to_string()<<'\n';
            continue;
        }

        // Get top tokens
        t2=numeric_stack.back();
        numeric_stack.pop_back();
        t1=numeric_stack.back();
        numeric_stack.pop_back();

        // Get parsed numbers
        p2= parsed_number(this->lexical_analyzer_,t2);
        p1= parsed_number(this->lexical_analyzer_,t1);

        if(p1.is_type(tokens::t_floats)||p2.is_type(tokens::t_floats))
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
        numeric_stack.push_back(result);

        if(this->debug_)
        {
            cout<<"Result: ";

            if(result->get_info()->is_type(tokens::t_integers))
                cout<<result->get_info()->get_int_data()->get_data()<<'\n';

            else if(result->get_info()->is_type(tokens::t_floats))
                cout<<result->get_info()->get_float_data()->get_data()<<'\n';
        }
        temp_tokens.push_back(result);
    }

    // Get final result token
    result=numeric_stack.front();

    // Deallocate temporary tokens
    for(auto& t:temp_tokens)
        if(t!=result)
        delete t;

    // Free memory
    delete postfix;

    if(this->debug_)
    {
        cout<<"Result: ";

        if(result->get_info()->is_type(tokens::t_integers))
            cout<<result->get_info()->get_int_data()->get_data()<<'\n';

        else if(result->get_info()->is_type(tokens::t_floats))
            cout<<result->get_info()->get_float_data()->get_data()<<'\n';
    }

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
