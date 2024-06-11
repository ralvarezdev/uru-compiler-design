#pragma once

#include <exception>
#include <string>

using std::exception;
using std::string;

class ExpressionsException : public exception
{
private:
  char *message;

public:
  // Constructor to set the exception message
  ExpressionsException(char *msg) : message(msg) {}

  ExpressionsException(string msg)
  {
    char msgChar[msg.length()];
    int idx = 0;

    for (char c : msg)
      msgChar[idx++] = c;

    this->message = msgChar;
  }

  // Display error message when the the exception is thrown
  char *what()
  {
    return message;
  }
};