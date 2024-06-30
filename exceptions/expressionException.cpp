#include "expressionException.hpp"

// Constructor to set the exception message
expression_exception::expression_exception(char *msg) : message_(msg) {}

expression_exception::expression_exception(const string* msg)
{
    this->message_ = new char[msg->length()];
    int idx = 0;

    for (const char c : msg)
        this->message_[idx++] = c;
}

// Display error message when the the exception is thrown
char *expression_exception::what()
{
    return message_;
}