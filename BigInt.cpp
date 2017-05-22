#include "BigInt.h"

#include <cstdlib>
#include <stdexcept>
#include <iostream>

BigInt::BigInt(std::string stringOfNumbers)
{
  /* Check that the entire string is an integer */
  if (stringOfNumbers.find_first_not_of("0123456789") != std::string::npos)
    throw std::invalid_argument(stringOfNumbers + " is not a valid number.");

  /* Gets each and every digit of the string */
  for (auto iii = stringOfNumbers.size() - 1; iii >= 0; iii--)
  {
    const char *digit = new char[2] {stringOfNumbers[iii], '\0'}; //gets the digit
    unsigned char theDigit = static_cast<char>(std::atoi(digit));
    digits.push_back(theDigit); //pushes back the digit to the vector
  }
}

void BigInt::quickPrint()
{
  std::cout << "The number is: ";
  for (auto iii = digits.size() - 1; iii >= 0; iii--)
    std::cout << digits[iii];

  std::cout << ".";
}
