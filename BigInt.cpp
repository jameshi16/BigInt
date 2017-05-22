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
  for (int iii = stringOfNumbers.size() - 1; iii >= 0; iii--)
  {
    const char *digit = new char[2] {stringOfNumbers.at(iii), '\0'}; //gets the digit
    std::cout << iii << std::endl;
    char theDigit = std::atoi(digit);
    //std::cout << theDigit << " " << static_cast<short>(theDigit) << std::endl;
    digits.push_back(theDigit); //pushes back the digit to the vector
    delete[] digit; //deletes the digit char array
  }
}

void BigInt::quickPrint()
{
  std::cout << "The number is: ";
  for (int iii = digits.size() - 1; iii >= 0; iii--)
    std::cout << static_cast<int>(digits.at(iii));

  std::cout << ".";
}
