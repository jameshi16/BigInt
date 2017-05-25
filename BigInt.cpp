#include "BigInt.h"

#include <cstdlib>
#include <stdexcept>
#include <iostream>

BigInt::BigInt(std::string stringOfNumbers)
{
  /* Check that the entire string is an integer */
  if (stringOfNumbers.find_first_not_of("0123456789") != std::string::npos)
    throw std::invalid_argument(stringOfNumbers + " is not a valid number.");

  /* Gets each and every base 100 digit of the string */
  for (int iii = stringOfNumbers.size() - 1; iii >= 0; iii--)
  {
    const char *digit = nullptr;

    if (iii - 1 >= 0)
    {
      digit = new char[3] {stringOfNumbers.at(iii - 1), stringOfNumbers.at(iii), '\0'}; //gets the base 100 digit
      iii--; //I've already processed the next digit, so I'll decrement once.
    }
    else
      digit = new char[2] {stringOfNumbers.at(iii), '\0'}; //gets the base 100 digit

    char theDigit = std::atoi(digit); //converts the digit into a digit
    digits.push_back(theDigit); //pushes back the digit to the vector
    delete[] digit; //deletes the digit char array
  }
}

BigInt::BigInt(const std::vector<unsigned char>& m_digits)
{
  //TODO: Check if the digits are valid
  digits = m_digits; //copies the vector
}

bool BigInt::operator>(BigInt& secondNumber) const
{
  if (digits.size() > secondNumber.digits.size())
    return true; //.size() returns the number of digits. The first set of digits contains more digits than the second set of digits

  if (polarity > secondNumber.polarity)
    return false; //the polarity is a dead give-away; if this number is -ve, it must be less than a +ve

  if (digits.size() == secondNumber.digits.size()) //if equal number of digits
  {
    for (unsigned int iii = digits.size(); iii >= 0; iii--) //read the numbers from the highest positional weight to the lowest positional weight
    {
      if (digits[0] > secondNumber.digits[0])
        return true;

      if (secondNumber.digits[0] > digits[0])
        return false;
    }
  }

  return false; //a common fallthrough
}

BigInt BigInt::operator+(BigInt secondNumber)
{
  std::vector<unsigned char> *largerDigits         = nullptr; //initialize the pointer to a nullptr
  const std::vector<unsigned char> *smallerDigits  = nullptr; //initialize the pointer to a nullptr (this code does NOT modify the smallerDigits array.)

  std::vector<unsigned char> currentDigits         = digits; //clones a copy of the digits within the this pointer (I did this to declare on stack (no need to use delete))

  /* Check which of the 2 numbers (this or secondNumber) has the larger amount of digits */
  if (digits.size() >= secondNumber.digits.size()) //if the current number has more digits
  {
    largerDigits  = &currentDigits; //set larger number to point to the digits
    smallerDigits = &(secondNumber.digits); //sets the smaller digit to point to the secondNumber's digits
  }
  else
  {
    largerDigits  = &(secondNumber.digits); //sets the larger digit to point to the secondNumber's digits
    smallerDigits = &currentDigits; //set larger number to point to the digits
  }

  /* Begin digit by digit addition */
  short carryOver = 0; //define the carry-over
  for (unsigned int iii = 0; iii < largerDigits->size(); iii++)
  {
    /** If the loop has already gone beyond the size of the smaller digit **/
    if (iii > (smallerDigits->size() - 1))
    {
      (*largerDigits)[iii] += carryOver; //adds the carry over to the current digit
      carryOver = 0; //resets the carry over

      if ((*largerDigits)[iii] >= 100) //carry over algorithm
      {
        (*largerDigits)[iii] -= 100; //gets, then sets the current digit to be 100 less.
        carryOver = 1; //sets a carry over

        if (iii == (largerDigits->size() - 1)) //if iii is at it's last digit, and there is a carry over
        {
          largerDigits->push_back(carryOver); //add a digit at the end
          break; //last digit, break out of loop.
        }

        continue; //skip all code below
      }

      break; //if no more carry over, breaks the loop, addition complete.
    }

    (*largerDigits)[iii] += carryOver; //adds the carry over first
    carryOver = 0; //resets the carry over

    (*largerDigits)[iii] += (*smallerDigits)[iii]; //adds the current digit from each array of digits with each other
    if ((*largerDigits)[iii] >= 100)
    {
      (*largerDigits)[iii] -= 100; //gets, then sets the current digit to be 10 less.
      carryOver = 1; //sets a carry over

      if (iii == (largerDigits->size() - 1)) //if iii is at it's last digit, and there is a carry over
      {
        largerDigits->push_back(carryOver); //add a digit at the end
        break; //last digit, break out of loop
      }
    }
  }

  /* Returns a new BigInt with the value of the largerDigits. */
  return BigInt(*largerDigits);
}

BigInt& BigInt::operator+=(BigInt secondNumber)
{
  *this = *this + secondNumber;
  return *this;
}

BigInt BigInt::operator-(BigInt secondNumber)
{
  BigInt *biggerNumber        = nullptr; //the pointer to the larger number
  const BigInt *smallerNumber = nullptr; //the pointer to the smaller number (never modify the smaller number)

  BigInt currentNumber        = *this; //gets the current number

  /* Determine which number is larger */
  if (secondNumber > currentNumber)
  {
    biggerNumber  = &secondNumber;
    smallerNumber = &currentNumber;
  }
  else
  {
    biggerNumber  = &currentNumber;
    smallerNumber = &secondNumber;
  }

  /* Do the subtraction */
  short carryOver = 0;
  for (unsigned int iii = 0; iii < biggerNumber->digits.size(); iii++)
  {
    /** If iii is larger than the smaller number's size - 1 **/
    if (iii > smallerNumber->digits.size() - 1)
    {
      biggerNumber->digits[iii] += carryOver; //"adds" (it'll actually subtract) the carryOver
      carryOver = 0; //resets the carryOver

      if (biggerNumber->digits[iii] < 0) //CarryOver algorithm
      {
        biggerNumber->digits[iii] += 100;
        carryOver = -1; //another carry over!
        continue;
      }

      break; //carry over is dealt with, nothing to do with the number anymore
    }

    biggerNumber->digits[iii] -= smallerNumber->digits[iii]; //subtracts the digit accordingly
    biggerNumber->digits[iii] += carryOver; //"adds" (it'll actually subtract) the carryOver
    carryOver = 0; //carryOver used, reset to 0

    if (biggerNumber->digits[iii] < 0) //CarryOver algorithm
    {
      biggerNumber->digits[iii] += 100;
      carryOver = -1; //another carry over!
    }
  }

  /* Determine if the final BigInt should be positive or negative */
  if (biggerNumber == &secondNumber)
  {
    BigInt returnValue(*biggerNumber); //creates the return value
    returnValue.polarity = 1; //toggles the polarity of the return value
    return returnValue; //returns the returnValue
  }
  return BigInt(*biggerNumber); //returns the larger BigInt, no polarity toggle
}

void BigInt::quickPrint()
{
  trim(); //trims the number
  std::cout << "The number is: ";
  for (int iii = digits.size() - 1; iii >= 0; iii--)
  {
    if (digits[iii] >= 0 && digits[iii] <= 9 && iii != digits.size() - 1)
    {
      std::cout << "0" << static_cast<int>(digits.at(iii));
      continue;
    }

    if (iii == digits.size() - 1 && polarity == 1)
    {
      std::cout << "-" << static_cast<int>(digits.at(iii));
      continue;
    }

    std::cout << static_cast<int>(digits.at(iii));
  }

  std::cout << ".";
}

bool BigInt::trim()
{
  bool clear = false;
  while (!clear)
  {
    for (unsigned int iii = digits.size() - 1; iii >= 0; iii--)
    {
      if (digits[iii] == 0) //if the digit is 0
      {
        digits.erase(digits.begin() + iii); //erases the element
        break;
      }

      if (digits[iii] != 0) //if a digit that is not 0 is reached
      {
        clear = true; //trimming complete
        break; //breaks out of the loop
      }
    }
  }
}
