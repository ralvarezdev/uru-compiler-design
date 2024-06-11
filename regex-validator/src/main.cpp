#include <vector>
#include <iostream>

#include "lib/exceptions/expressionsException.h"
#include "lib/validators/expressions.h"

using std::cout;
using std::vector;

using namespace regex;

int main()
{
  vector<string> wordToCheck({"1", "a", "818", "asbdasA", "12312.1231", "asda712", "83812asdas", "asda7ssa$", "23712$))%)(&$", "(a]"});

  // Loop to test the RegEx validator implementation
  for (string word : wordToCheck)
    try
    {

      if (regex::isValid(word))
        cout << "VALID: " << word << "\n";

      else
        cout << "MATCH NOT FOUND: " << word << "\n";
    }
    catch (ExpressionsException e)
    {
      cout << e.what() << '\n';
    }

  return 0;
}