#pragma once

#include <exception>
#include <string>

using std::exception;
using std::string;

class expression_exception final : public exception
{
  const char* message_{};

public:
  explicit expression_exception(const char* msg): message_(msg)
  {
  }
  explicit expression_exception(const string&);

  ~expression_exception() override
  {
    delete message_;
  }


  const char* what();
};
