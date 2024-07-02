#include "expressionException.hpp"

// Display error message when the the exception is thrown
const char* expression_exception::what()
{
    return message_;
}
