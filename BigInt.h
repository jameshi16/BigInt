#ifndef BIGINT_H
#define BIGINT_H

/*Standard Includes*/
#include <vector> //the storage means of the BigInt
#include <string> //for string

/**
 * BigInt class. This class stores numbers in Base 100, and uses
 * unsigned char to store the said numbers.
 */
class BigInt
{
public:
  BigInt()=default; //default constructor for big int

  /**
   * A constructor that takes in a string of numbers as input.
   * @param stringOfNumbers The string of numbers
   */
  BigInt(std::string stringOfNumbers); //a constructor taking in a string
  /**
   * A constructor that takes in a vector of unsigned chars as input. (unsigned chars use to store numbers instead of ints)
   * @param m_digits The digits of numbers
   */
  BigInt(const std::vector<unsigned char>& m_digits); //a constructor taking in the digits form
  /**
   * A constructor that takes in a actual number (up to long long) as input
   * @param  integer The said number
   */
  BigInt(long long integer); //a constructor taking in an actual number

  BigInt(const BigInt&)=default; //default copy constructor
  BigInt(BigInt&&)=default; //default move constructor
  ~BigInt()=default; //default destructor for big int

  /**
   * Copy constructor (default)
   */
  BigInt& operator=(const BigInt&)=default;
  /**
   * Move constructor (default)
   */
  BigInt& operator=(BigInt&&)=default;

  /**
   * Relational: greater than
   */
   bool operator>(const BigInt& secondNumber) const;

  /**
   * Relational: less than
   */
  bool operator<(const BigInt& secondNumber) const;

  /**
   * Relational: equality
   */
  bool operator==(const BigInt& secondNumber) const;

  /**
   * Relational: opposite of equality
   */
  bool operator!=(const BigInt& secondNumber) const;

  /**
   * Relational: less than and equal to
   */
  bool operator<=(const BigInt& secondNumber) const;

  /**
   * Relational: greater than and equal to
   */
  bool operator>=(const BigInt& secondNumber) const;

  /**
   * The + operation of BigInt. Takes the second number and adds it to the current number, returning the new number
   * @param secondNumber The second BigInt to add to
   */
  BigInt operator+(BigInt secondNumber);

  /**
   * The ++ operation of BigInt. Increments the current number
   */
  BigInt& operator++();

  /**
   * The ++ (postfix) operation of BigInt. Returns the current number, but increments it.
   */
  BigInt operator++(int);

  /**
   * The -- (postfix) operation of BigInt. Returns the current number, but decrements it.
   */
  BigInt operator--(int);

  /**
   * The -- operation of BigInt. Decrements the current number
   */
  BigInt& operator--();

  /**
   * The += operation of BigInt. Takes the second number and adds it to the current number, returning the current number.
   * @param secondNumber The second BigInt to add to the current number
   */
  BigInt& operator+=(BigInt secondNumber);

  /**
   * The - operation of BigInt. Takes the second number and subtracts it from the current number, returning the new number
   * @param secondNumber The second BigInt to subtract to
   */
  BigInt operator-(BigInt secondNumber);

  /**
   * The operator-=. Takes the second number and subtracts it from the current number, returning the current number
   * @param secondNumber The second BigInt to subtract to
   */
  BigInt& operator-=(BigInt secondNumber);

  /**
   * The operator*. Takes the second number as multiplier.
   * @param secondNumber The second BigInt to multiply to.
   */
  BigInt operator*(BigInt secondNumber);

  /**
   * The operator*=. Takes the second number as the multiplier, multiplying and returning the current number
   * @param secondNumber The second BigInt to multiply to.
   */
  BigInt& operator*=(BigInt secondNumber);

  /**
   * The operator/. Takes the second number as the divider.
   * @param secondNumber The second BigInt to divide to.
   */
  BigInt operator/(BigInt secondNumber);

  /**
   * The operator/=. Takes the second number as the divider, dividing and returning the current number
   * @param secondNumber The second BigInt to divide to
   */
  BigInt& operator/=(BigInt secondNumber);

  /**
   * The operator%. Takes the second number as the divider.
   * @param secondNumber The second BigInt to divide to.
   */
  BigInt operator%(BigInt secondNumber);

  /**
   * Unary (-) overloading
   */
  BigInt& operator-();


  void quickPrint(); //a convenient function to use while developing to print the number to output stream
  void print(); //prints only the number to the output stream
  /**
   * Trims the digits
   * @return Returns true if there were things to trim, false if there were not things to trim
   */
  bool trim(); //trim the digits
protected:
private:
  std::vector<unsigned char> digits = {}; //the digits of the BigInt. The digit with the highest positional weight is the last one.
  //I use unsigned char because I don't want my char to have a sign.

  /**
   * Initializes the class from string
   * @param stringOfNumbers A string
   */
  void initFromString(std::string stringOfNumbers);

  bool polarity = 0; //0 is positive, 1 is negative.
};

#endif //end of header guard BIGINT
