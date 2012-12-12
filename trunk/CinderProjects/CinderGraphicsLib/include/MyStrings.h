#pragma once

#include "cinder/Utilities.h"
#include <string>

class MyString
{
public:

  static std::string getFrameNumber (std::string name, size_t number)
  {
    std::string numberString;
    std::string zeroString;

    numberString = ci::toString(number);

    for (size_t i = 0; i < 4 - numberString.length(); i++)
      zeroString.insert (zeroString.end(), '0');

  	return name + zeroString + numberString;	
  }

};
