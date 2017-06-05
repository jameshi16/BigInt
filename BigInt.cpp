#include "BigInt.h"

#include <cstdlib>
#include <stdexcept>
#include <iostream>
#include <sstream>

BigInt::BigInt(std::string stringOfNumbers)
{
  initFromString(stringOfNumbers); //uses the private method to initialize the number
}

BigInt::BigInt(const std::vector<unsigned char>& m_digits)
{
  //TODO: Check if the digits are valid
  digits = m_digits; //copies the vector
}

BigInt::BigInt(long long integer)
{
  std::stringstream ss;
  ss << integer;
  initFromString(ss.str()); //initializes from string
}

bool BigInt::operator>(const BigInt& secondNumber) const
{
  if (digits.size() > secondNumber.digits.size())
    return true; //.size() returns the number of digits. The first set of digits contains more digits than the second set of digits

  if (polarity > secondNumber.polarity)
    return false; //the polarity is a dead give-away; if this number is -ve, it must be less than a +ve

  if (digits.size() == secondNumber.digits.size()) //if equal number of digits
  {
    for (long iii = digits.size() - 1; iii >= 0; iii--) //read the numbers from the highest positional weight to the lowest positional weight
    {
      if (digits[iii] > secondNumber.digits[iii])
        return true;

      if (secondNumber.digits[iii] > digits[iii])
        return false;
    }
  }

  return false; //a common fallthrough
}

bool BigInt::operator<(const BigInt& secondNumber) const
{
  return (secondNumber > *this); //uses the greater than to figure out
}

bool BigInt::operator==(const BigInt& secondNumber) const
{
  /* Checks digits */
  if (digits.size() != secondNumber.digits.size())
    return false; //definitely not equal

  /* Checks all the digits of both numbers */
  for (auto iii = 0; iii < this->digits.size(); iii++) //loops through all the digits of the current number
  {
    if (this->digits[iii] != secondNumber.digits[iii])
      return false; //is not equal
  }
  return true; //is equal
}

bool BigInt::operator!=(const BigInt& secondNumber) const
{
  return !(*this == secondNumber);
}

bool BigInt::operator<=(const BigInt& secondNumber) const
{
  if (*this < secondNumber || *this == secondNumber)
    return true; //returns true

  return false; //returns false
}

bool BigInt::operator>=(const BigInt& secondNumber) const
{
  if (*this > secondNumber || *this == secondNumber)
    return true; //returns true

  return false; //returns false.
}

BigInt BigInt::operator+(BigInt secondNumber)
{
  /* Redirect the operation if all of the numbers have a polarity of 1 */
  if (polarity == 1 && secondNumber.polarity == 1)
  {
    -*this; -secondNumber; //flips the negative sign
    return -(*this + secondNumber); //returns the negative of the sum of the numbers when positive
  }

  /* Redirect the operation if either of the numbers have a polarity of 1 */
  if (polarity == 0 && secondNumber.polarity == 1)
  {
    -secondNumber; //swap the polarity of the number
    return (*this - secondNumber); //this will handle everything
  }

  if (polarity == 1 && secondNumber.polarity == 0)
  {
    -*this; //swap the polarity of the number
    return (secondNumber - *this); //this will handle everything
  }

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

  /* Returns a new BigInt with the value of the largerDigits. (After trimming) */
  BigInt returnValue = BigInt(*largerDigits);
  returnValue.trim(); //trims the return value
  return returnValue;
}

BigInt& BigInt::operator++()
{
  return *this = *this + BigInt("1");
}

BigInt BigInt::operator++(int)
{
  BigInt returnValue = *this; //sets the return value
  ++*this; //increments
  return returnValue;
}

BigInt BigInt::operator--(int)
{
  BigInt returnValue = *this; //sets the return value
  --*this; //decrements
  return returnValue;
}

BigInt& BigInt::operator--()
{
  return *this = *this - BigInt("1");
}

BigInt& BigInt::operator+=(BigInt secondNumber)
{
  *this = *this + secondNumber;
  return *this;
}

BigInt BigInt::operator-(BigInt secondNumber)
{
  /* Redirect the operation to subtraction if all polarity is 1 */
  if (polarity == 1 && secondNumber.polarity == 1)
  {
    -secondNumber; //swaps the polarity
    return (secondNumber - *this); //this will handle everything
  }

  /* Redirect the operation to addition if either polarity is 1 */
  if (polarity == 1 && secondNumber.polarity == 0)
  {
    -*this; -secondNumber; //swaps the polarity
    return -(*this + secondNumber); //adds the two values together, and then swaps the polarity
  }

  if (polarity == 0 && secondNumber.polarity == 1) //redirect to addition
  {
    -secondNumber; //swaps the polarity
    return (*this + secondNumber); //basically adding the two numbers together
  }

  BigInt *biggerNumber        = nullptr; //the pointer to the larger number
  const BigInt *smallerNumber = nullptr; //the pointer to the smaller number (never modify the smaller number)

  BigInt currentNumber        = *this; //gets the current number

  /* Determine which number is larger */
  if (secondNumber >= currentNumber)
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
  short currentDigit = 0;
  for (unsigned int iii = 0; iii < biggerNumber->digits.size(); iii++)
  {
    currentDigit = biggerNumber->digits[iii]; //need to store it in short, so that I can deal with negative numbers
    /** If iii is larger than the smaller number's size - 1 **/
    if (iii > smallerNumber->digits.size() - 1)
    {
      currentDigit += carryOver; //"adds" (it'll actually subtract) the carryOver
      carryOver = 0; //resets the carryOver

      if (currentDigit < 0) //CarryOver algorithm
      {
        currentDigit += 100;
        carryOver = -1; //another carry over!
        biggerNumber->digits[iii] = currentDigit; //sets the current digit of bigger number
        currentDigit = 0; //sets the current digit back to 0
        continue;
      }

      biggerNumber->digits[iii] = currentDigit; //sets the current digit of the bigger number
      currentDigit = 0; //sets the current digit back to 0

      break; //carry over is dealt with, nothing to do with the number anymore
    }

    currentDigit -= smallerNumber->digits[iii]; //subtracts the digit accordingly
    currentDigit += carryOver; //"adds" (it'll actually subtract) the carryOver
    carryOver = 0; //carryOver used, reset to 0

    if (currentDigit < 0) //CarryOver algorithm
    {
      currentDigit += 100;
      carryOver = -1; //another carry over!
    }

    biggerNumber->digits[iii] = currentDigit; //sets the current digit of bigger number
    currentDigit = 0; //sets the current digit back to 0
  }

  BigInt returnValue(*biggerNumber); //creates the returnValue
  returnValue.trim(); //trims the returnValue
  /* Determine if the final BigInt should be positive or negative */
  if (biggerNumber == &secondNumber)
  {
    returnValue.polarity = 1; //toggles the polarity of the return value
    return returnValue; //returns the returnValue
  }
  return returnValue; //returns the larger BigInt, no polarity toggle
}

BigInt& BigInt::operator-=(BigInt secondNumber)
{
  return *this = *this - secondNumber;
}

BigInt& BigInt::operator-()
{
  polarity = (polarity) ? (0) : (1); //flips the polarity
  return *this;
}

BigInt BigInt::operator*(BigInt secondNumber)
{
  /* Variable declaration */
  bool finalPolarity = 0; //determines the final polarity of the number

  BigInt *largerNumber  = nullptr; //the larger bigInt
  BigInt *smallerNumber = nullptr; //the smaller bigInt
  BigInt returnValue("0"); //the return value

  BigInt currentNumber = *this; //makes a copy of this to currentNumber

  /* Size checking */
  if (secondNumber >= currentNumber)
  {
    largerNumber  = &secondNumber;
    smallerNumber = &currentNumber;
  }
  else
  {
    largerNumber  = &currentNumber;
    smallerNumber = &secondNumber;
  }

  /* Polarity checking */
  if (smallerNumber->polarity == 1 && largerNumber->polarity == 1)
  {
    -*this; -secondNumber; //swaps the polarity
    finalPolarity = 0; //is positive
  }

  if (smallerNumber->polarity == 0 && largerNumber->polarity == 1)
  {
    -secondNumber; //swaps the polarity
    finalPolarity = 1; //is negative
  }

  if (smallerNumber->polarity == 1 && largerNumber->polarity == 0)
  {
    -*this; //swaps the polarity
    finalPolarity = 1; //is negative
  }

  unsigned short carryOver = 0; //the carry over (this is 2 bytes, so it should be able to store 99 * 99 no problem)
  /* Attempt multiplication */
  for (unsigned int iii = 0; iii < smallerNumber->digits.size(); iii++)
  {
    BigInt bigBuffer = *largerNumber; //replicates largerNumber in this multiplcation
    unsigned short multiplierBuffer = smallerNumber->digits.at(iii); //stores the smallerNumber in a buffer
    for (unsigned int jjj = 0; jjj < largerNumber->digits.size(); jjj++)
    {
      unsigned short multiplicand = largerNumber->digits.at(jjj); //stores the largerNumber in a buffer
      multiplicand *= multiplierBuffer; //to save space, put the multiplied value into the mulplicand

      multiplicand += carryOver; //adds the carry over after multiplication
      carryOver = 0; //resets the carry over


      if (multiplicand > 99) //carry over algorithm
      {
        carryOver = multiplicand / 100; //gets the carry over, which is the multiplicand divided by the base number, 100. NOTE: int discards decimals
        bigBuffer.digits.at(jjj) = multiplicand % (100 * carryOver); //gets the remainder and then assigns it to the larger number
      }
      else bigBuffer.digits.at(jjj) = multiplicand; //if not, just make the multiplicand the answer

      //Handler for carry over if first positional weight digit has been reached
      if (jjj == largerNumber->digits.size() - 1 && carryOver != 0)
      {
        bigBuffer.digits.push_back(carryOver); //pushes back the carry over
        carryOver = 0; //resets the carry over
        break; //breaks the loop
      }
    }

    /** Add iii zeros behind the buffer, then add it to the returnValue **/
    for (unsigned int n_loop = 0; n_loop < iii; n_loop++)
      bigBuffer.digits.insert(bigBuffer.digits.begin(), 0); //add a zero for each loop

    returnValue += bigBuffer; //adds the buffer to the return value
  }
  returnValue.polarity = finalPolarity; //changes the polarity to the correct one
  returnValue.trim(); //trims the returnValue
  return returnValue; //returns the calculated number
}

BigInt& BigInt::operator*=(BigInt secondNumber)
{
  return *this = *this * secondNumber; //returns the value of this after multiplying this to the second number
}

BigInt BigInt::operator/(BigInt secondNumber)
{
  BigInt dividend   = *this; //makes n_1 the dividend
  BigInt& divider   = secondNumber; //makes n_2 the divider
  BigInt buffer("0"); //creates an empty buffer
  BigInt quotient("0"); //the quotient

  bool finalPolarity = false; //the final polarity of the result

  /* Polarity checking */
  if (dividend.polarity == 1 && divider.polarity == 1)
  {
    -dividend; -divider; //swaps the polarity
    finalPolarity = 0; //is positive
  }

  if (dividend.polarity == 0 && divider.polarity == 1)
  {
    -divider; //swaps the polarity
    finalPolarity = 1; //is negative
  }

  if (dividend.polarity == 1 && divider.polarity == 0)
  {
    -dividend; //swaps the polarity
    finalPolarity = 1; //is negative
  }

  /* Check for any special cases */
  if (divider == BigInt("0"))
  {
    std::logic_error("BigInt failed: Error 428, division by 0.");
    return *this; //returns the unharmed number
  }

  if (divider == BigInt("1"))
    return *this; //anything divided by 1 is anything

  if (divider > dividend)
    return BigInt(0); //returns 0

  if (divider == dividend)
    return BigInt(1); //returns 1

  /* Perform division */
  for (long iii = dividend.digits.size() - 1; iii >= 0; iii--) //I used long here because long > int
  {
    buffer.digits.insert(buffer.digits.begin(), dividend.digits.at(iii)); //pushes back the digit into the buffer
    buffer.trim(); //trims the BigInt, removing the first 0.
    if (buffer < divider)
    {
      quotient.digits.insert(quotient.digits.begin(), 0); //inserts a 0 at the back of the number (a.k.a the front of the vector)
      continue; //skips to the next iteration
    }

    /* Buffer >= Divider, ready for division */
    for (unsigned int bi_loop = 1; bi_loop < 99; bi_loop++) //loops through all the numbers from 1 to 99
    {
      if (BigInt(bi_loop) * divider > buffer) //if bi_loop * divider is more than the buffer,
      {
        quotient.digits.insert(quotient.digits.begin(), bi_loop - 1); //adds bi_loop - 1 into the quotient
        buffer -= BigInt(bi_loop - 1) * divider; //subtracts from the buffer - this should give a 1 digit answer (in base 10)
        break; //break out of the loop
      }
      if (BigInt(bi_loop) * divider == buffer) //if bi_loop * divider is exactly equal to the buffer
      {
        quotient.digits.insert(quotient.digits.begin(), bi_loop); //adds bi_loop into the quotient
        buffer -= BigInt(bi_loop) * divider; //subtracts fromt he buffer - this should give a 1 digit andswer
        break; //break out of loop
      }
    }
  }

  /* Anything left in the buffer is the remainder, but we don't need that */
  quotient.trim(); //trims the quotient
  quotient.polarity = finalPolarity;
  return quotient;
}

BigInt& BigInt::operator/=(BigInt secondNumber)
{
  return *this = *this / secondNumber;
}

BigInt BigInt::operator%(BigInt secondNumber)
{
  BigInt dividend   = *this; //makes n_1 the dividend
  BigInt& divider   = secondNumber; //makes n_2 the divider
  BigInt buffer("0"); //creates an empty buffer

  bool finalPolarity = false; //the final polarity of the result

  /* Polarity checking */
  if (dividend.polarity == 1 && divider.polarity == 1)
  {
    -dividend; -divider; //swaps the polarity
    finalPolarity = 0; //is positive
  }

  if (dividend.polarity == 0 && divider.polarity == 1)
  {
    -divider; //swaps the polarity
    finalPolarity = 1; //is negative
  }

  if (dividend.polarity == 1 && divider.polarity == 0)
  {
    -dividend; //swaps the polarity
    finalPolarity = 1; //is negative
  }

  /* Check for any special cases */
  if (divider == BigInt("0"))
  {
    std::logic_error("BigInt failed: Error 428, division by 0.");
    return *this; //returns the unharmed number
  }

  if (divider == BigInt("1"))
    return *this; //anything divided by 1 is anything

  if (divider > dividend)
    return BigInt(0); //returns 0

  if (divider == dividend)
    return BigInt(1); //returns 1

  /* Perform division */
  for (long iii = dividend.digits.size() - 1; iii >= 0; iii--) //I used long here because long > int
  {
    buffer.digits.insert(buffer.digits.begin(), dividend.digits.at(iii)); //pushes back the digit into the buffer
    buffer.trim(); //trims the BigInt, removing the first 0.
    if (buffer < divider)
      continue; //skips to the next iteration

    /* Buffer >= Divider, ready for division */
    for (unsigned int bi_loop = 1; bi_loop < 99; bi_loop++) //loops through all the numbers from 1 to 99
    {
      if (BigInt(bi_loop) * divider > buffer) //if bi_loop * divider is more than the buffer,
      {
        buffer -= BigInt(bi_loop - 1) * divider; //subtracts from the buffer - this should give a 1 digit answer (in base 10)
        break; //break out of the loop
      }
      if (BigInt(bi_loop) * divider == buffer) //if bi_loop * divider is exactly equal to the buffer
      {
        buffer -= BigInt(bi_loop) * divider; //subtracts fromt he buffer - this should give a 1 digit andswer
        break; //break out of loop
      }
    }
  }

  /* Anything left in the buffer is the remainder, but we don't need that */
  buffer.trim(); //trims the quotient
  buffer.polarity = finalPolarity;
  return buffer;
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

  std::cout << "." << std::endl;
}

void BigInt::print()
{
  trim(); //trims the number
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
}

bool BigInt::trim()
{
  bool clear = false;
  while (!clear)
  {
    for (long iii = digits.size() - 1; iii >= 0; iii--)
    {
      if (iii == 0 && digits[iii] == 0) //specifically made if there is only 1 single digit, and the digit is 0
      {
        clear = true;
        break; //breaks immediately
      }

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

void BigInt::initFromString(std::string stringOfNumbers)
{
  /* Check that the entire string is an integer */
  if (stringOfNumbers.find_first_not_of("-0123456789") != std::string::npos)
    throw std::invalid_argument(stringOfNumbers + " is not a valid number. Reason: foreign characters");

  if (stringOfNumbers.find("--") != std::string::npos)
    throw std::invalid_argument(stringOfNumbers + " is not a valid number. Reason: multiple -");

  if (stringOfNumbers.find_first_of("-") != std::string::npos)
  {
    -*this; //swaps the polarity
    stringOfNumbers.erase(stringOfNumbers.begin() + stringOfNumbers.find_first_of("-"));
  }

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
