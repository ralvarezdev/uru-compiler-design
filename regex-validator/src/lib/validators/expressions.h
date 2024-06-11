#pragma once

#include <string>
#include <format>

using std::format;
using std::string;

namespace regex
{
  namespace
  {
    // - Private enums
    enum regexType
    {
      DELIMETER,
      CAPTURE,
      SET,
      OP,
      QT,
      UNDEFINED
    };

    // - Private constants

    // -- Characters limit
    const int MAX_CHAR = 4096;

    // -- Expression-related character constants
    const int BEGINNING = '^';
    const int END = '$';

    const int NEGATED_OP = '^';

    // Set-related
    const int SET_START = '[';
    const int SET_END = ']';

    // Range-related
    const int RANGE_OP = '-';

    // Quantifier-related
    const int QT_NULLABLE = '*';
    const int QT_OPTIONAL = '?';
    const int QT_NON_NULLABLE = '+';

    const int QT_TIMES_START = '{';
    const int QT_TIMES_END = '}';
    const int QT_TIMES_SEP = ',';

    // Capture group-related
    const int CAPTURE_START = '(';
    const int CAPTURE_END = ')';

    // Logical OR
    const int OR_OP = '|';

    // -- Character classes

    // Anything except newline (.)
    const string CLASS_DOT = "[^\n\r]";

    // Word character (\w)
    const string CLASS_WORD = "[a-zA-Z0-9_]";

    // Non-word character (\W)
    const string CLASS_NON_WORD = "[^a-zA-Z0-9_]";

    // Digits (\d)
    const string CLASS_DIGIT = "[0-9]";

    // Non-digits (\D)
    const string CLASS_NON_DIGIT = "[^0-9]";

    // Whitespace characters (\s)
    const string CLASS_WHITESPACE = "[ \t\r\n\f]";

    // Non-whitespace characters (\S)
    const string CLASS_NON_WHITESPACE = "[^ \t\r\n\f]";

    // -- Project-related classes

    // Project digits
    const string PR_CLASS_DIGIT = CLASS_DIGIT;

    // Project alphabetical character
    const string PR_CLASS_ALPHA = "[a-zA-Z]";

    // Project whole number
    const string PR_CLASS_WHOLE_NUM = format("{}+", PR_CLASS_DIGIT);

    // Project word
    const string PR_CLASS_WORD = format("{}+", PR_CLASS_ALPHA);

    // Project real number
    const string PR_CLASS_REAL_NUM = format("{0}.{0}", PR_CLASS_DIGIT);

    // Project identifier
    const string PR_CLASS_IDENTIFIER = format("{0}({1}|{0})*", PR_CLASS_ALPHA, PR_CLASS_DIGIT);

    // - Execeptions

    // Max size
    const string MAX_CHAR_MSG = "MAX CHAR: String size is bigger than the max size";

    // Missing set
    const string MISSING_SET_START_MSG = "MISSING SET: There's no set before the range operator.";
    const string MISSING_SET_END_MSG = "MISSING SET: Set not closed.";

    // Missing times
    const string MISSING_TIMES_START_MSG = "MISSING TIMES: There's no quantity times operator before the separator.";
    const string MISSING_TIMES_END_MSG = "MISSING TIMES: Quantity times operator not closed.";

    // Missing capture group
    const string MISSING_CAPTURE_START_MSG = "MISSING CAPTURE: There's no capture group.";
    const string MISSING_CAPTURE_END_MSG = "MISSING CAPTURE: Capture group not closed.";

    // Mismatched input
    const string MISMATCHED_MSG = "MISMATCHED: Input string isn't valid by the given regular expression.";
  }

  // - Public function prototypes
  bool isDigit(string);
  bool isDigit(char);
  bool isAlpha(char);
  bool isAlpha(string);
  bool isWholeNumber(string);
  bool isWord(string);
  bool isRealNumber(string);
  bool isIdentifier(string);
  bool isValid(string);

  bool isEmpty(string);
  bool checkExpression(string *, const string *);

  /*
    bool isCapture(int);
    bool isSet(int);
  */
}