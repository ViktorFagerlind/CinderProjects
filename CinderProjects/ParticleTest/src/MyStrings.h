#pragma once

#include <string>

class MyString
{
public:

  static std::string toString (size_t number)
  {
    char rawNumberString[20];

    sprintf(rawNumberString, "%u", number);

    return std::string(rawNumberString);
  }
};
