#include "expressionException.hpp"

#include <cstring>

// - Expression exception class

// Constructor
expression_exception::expression_exception(const string& msg)
{
    auto* msg_char_array = new char[msg.length() + 1];
    strcpy(msg_char_array, msg.c_str());

    throw expression_exception(msg_char_array);
}

// Display error message when the the exception is thrown
const char* expression_exception::what()
{
    return message_;
}

