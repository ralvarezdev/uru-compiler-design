#include <iostream>
#include <string>
#include <vector>
#include <stack>

#include "expressions.h"
#include "../exceptions/expressionsException.h"

using std::cout;
using std::format;
using std::size_t;
using std::stack;
using std::string;
using std::vector;

using namespace regex;

// - RegEx namespace functions

// Function to check if the string is a digit
bool regex::isDigit(string input)
{
  return regex::checkExpression(&input, &regex::PR_CLASS_DIGIT);
}

bool regex::isDigit(char input)
{
  string inputStr(1, input);
  return isDigit(inputStr);
}

// Function to check if the string is a alphabetical character
bool regex::isAlpha(string input)
{
  return regex::checkExpression(&input, &regex::PR_CLASS_ALPHA);
}

bool regex::isAlpha(char input)
{
  string inputStr(1, input);
  return isAlpha(inputStr);
}

// Function ot check if the string is a whole number
bool regex::isWholeNumber(string input)
{
  return regex::checkExpression(&input, &regex::PR_CLASS_WHOLE_NUM);
}

// Function to check if the string is a word
bool regex::isWord(string input)
{
  return regex::checkExpression(&input, &regex::PR_CLASS_WORD);
}

// Function to check if the string is a real number
bool regex::isRealNumber(string input)
{
  return regex::checkExpression(&input, &regex::PR_CLASS_REAL_NUM);
}

// Function to check if the string is a identifier
bool regex::isIdentifier(string input)
{
  return regex::checkExpression(&input, &regex::PR_CLASS_IDENTIFIER);
}

// RegEx main validator
bool regex::isValid(string input)
{
  int length = input.length();

  // Check if the string is empty
  if (length == 0)
    return false;

  // Check if the string is a digit or an alphabetical character
  if (length == 1)
  {
    if (isDigit(input))
      return true;

    if (isAlpha(input))
      return true;

    return false;
  }

  // Given that the first character is alphabetical, check if the string is either a word or an identifier
  if (isAlpha(input.at(0)))
  {
    if (isWord(input))
      return true;

    if (isIdentifier(input))
      return true;

    return false;
  }

  // Given that the first character is a digit, check if the string is either a whole number or a real number
  if (isDigit(input.at(0)))
  {
    if (isWholeNumber(input))
      return true;

    if (isRealNumber(input))
      return true;

    return false;
  }

  return false;
}

// Function to check if string is empty
bool regex::isEmpty(string *input)
{
  if (input->empty())
    return true;

  return false;
}

// RegEx checker
bool regex::checkExpression(string *input, string *regex)
{
  int inputLength = input->length(), regexLength;
  int c, r, rNext, acc;
  int inputPos, regexPos;

  regexType top, regexOp;

  bool isInsideOp = false, isValid = false;

  string *temp = new string("");
  stack<regexType> depth;
  vector<string *> isoRegexs;

  // Check if the string is empty
  if (inputLength == 0)
    return false;

  // Check if the string might be too long
  if (inputLength >= regex::MAX_CHAR)
    throw ExpressionsException(regex::MAX_CHAR_MSG);

  // Check if there are multiple RegEx in a single one
  for (int r : *regex)
  {
    // Add character to temporary string
    temp->insert(temp->end(), c);

    // Get last operator that has been added
    top = depth.size() > 0 ? depth.top() : regex::UNDEFINED;

    // Opening operators
    if (r == SET_START && (top == regex::UNDEFINED || top == regex::CAPTURE))
      isValid = true;

    else if (r == CAPTURE_START && (top == regex::UNDEFINED || top == regex::CAPTURE))
      isValid = true;

    else if (r == QT_TIMES_START && (top == regex::UNDEFINED || top == regex::CAPTURE))
      isValid = true;

    if (isValid)
    {
      depth.push((regexType)r);
      continue;
    }

    // Separators
    if (r == regex::RANGE_OP)
    {
      if (top == regex::SET_START)
        continue;

      throw ExpressionsException(regex::MISSING_SET_START_MSG);
    }

    else if (r == regex::QT_TIMES_SEP)
    {
      if (top == regex::QT_TIMES_START)
        continue;

      throw ExpressionsException(regex::MISSING_TIMES_START_MSG);
    }

    // Closing operators
    else if (r == regex::SET_END)
    {
      if (top != regex::SET_START)
        throw ExpressionsException(regex::MISSING_SET_END_MSG);

      depth.pop();
    }

    else if (r == regex::CAPTURE_END)
    {
      if (top != regex::CAPTURE_START)
        throw ExpressionsException(regex::MISSING_CAPTURE_END_MSG);

      depth.pop();
    }

    else if (r == regex::QT_TIMES_END)
    {
      if (top != regex::QT_TIMES_START)
        throw ExpressionsException(regex::MISSING_TIMES_END_MSG);

      depth.pop();
    }

    // Logical OR
    else if (r == OR_OP && depth.size() == 0)
    {
      // Insert RegEx at the end of the vector
      isoRegexs.insert(isoRegexs.end(), temp);
      temp = new string("");
    }
  }

  // Insert accumulated RegEx at temporary string
  isoRegexs.insert(isoRegexs.end(), temp);

  // Check if the string matches with any of the regular expressions
  for (string *isoRegex : isoRegexs)
    try
    {
      inputPos = 0, regexPos = 0;
      regexLength = regex->length();

      isInsideOp = false;

      // Check if there's an operator for the beginning
      // TO DEVELOP

      // Check if there's a beginning operator
      while (inputPos < inputLength)
      {
        acc = 0;
        c = input->at(inputPos);
        r = isoRegex->at(regexPos);

        if (!isInsideOp)
        {
          regexOp = UNDEFINED;

          if (c == CAPTURE_START)
            regexOp = CAPTURE;

          else if (c == SET_START)
            regexOp = SET;

          else if (c == QT_TIMES_START)
            regexOp = QT;

          if (regexOp != UNDEFINED)
          {
            isInsideOp = true;
            continue;
          }

          // Get next character from the regular expression
          int diff = inputLength - inputPos;

          if (diff > 1)
            rNext = isoRegex->at(regexPos + 1);

          if (diff == 1 || (rNext != regex::QT_NULLABLE && rNext != regex::QT_NON_NULLABLE))
          {
            if (regex->at(regexPos++) != input->at(inputPos++))
              throw ExpressionsException(MISMATCHED_MSG);

            continue;
          }

          while (regex->at(regexPos) == input->at(inputPos++))
            acc++;

          regexPos += 2;

          if (rNext == regex::QT_NON_NULLABLE && acc == 0)
            throw ExpressionsException(MISMATCHED_MSG);

          continue;
        }

        // Set, Quantity times, Capture Group operators handler
        // TO DEVELOP

        // Check if there's an operator for the end
        // TO DEVELOP
      }

      return inputPos == inputLength && regexPos == regexLength;
    }
    catch (ExpressionsException e)
    {
      // Continue looping
    }

  return false;
}

/*
// Function to check if the given character is linked with a RegEx operation
bool regex::isEnclosingStart(int c)
{
  return c == regex::CAPTURE_START || c == regex::SET_START||c==regex::QT_TIMES_START;
}

  if (c == regex::SET_START || c == regex::SET_END)
regexType regex::isOp(int c)
{
  if (c == regex::BEGINNING || c == regex::END)
    return DELIMETER;

  if ()
    return CAPTURE;

    return SET;

  if (c == regex::NEGATED_OP || c == regex::RANGE_OP || c == regex::OR_OP)
    return OP;

  if (c == regex::QT_NON_NULLABLE || c == regex::QT_NULLABLE || c == regex::QT_OPTIONAL)
    return QT_OP;

  if (c == regex::QT_TIMES_START || c == regex::QT_TIMES_SEP || c == regex::QT_TIMES_END)
    return QT_OP;

  return UNDEFINED;
}

regexType regex::isOp(char c)
{
  return regex::isOp(c);
}
*/