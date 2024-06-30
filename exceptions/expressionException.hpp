#pragma once

#include <exception>
#include <string>

using std::exception;
using std::string;

class expression_exception final : public exception
{
private:
  char *message_;

public:
  explicit expression_exception(char *msg);
  explicit expression_exception(const string* msg);
  char *what();
};