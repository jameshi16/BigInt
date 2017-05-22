#ifndef BIGINT_H
#define BIGINT_H

/*Standard Includes*/
#include <vector> //the storage means of the BigInt
#include <string> //for string

class BigInt
{
public:
  BigInt()=default; //default constructor for big int

  /**
   * A constructor that takes in a string of numbers as input.
   * @param stringOfNumbers The string of numbers
   */
  BigInt(std::string stringOfNumbers); //a constructor taking in a string

  BigInt(const BigInt&)=default; //default copy constructor
  BigInt(BigInt&&)=default; //default move constructor
  ~BigInt()=default; //default destructor for big int

  void quickPrint(); //a convenient function to use while developing to print the number to output stream
protected:
private:
  std::vector<unsigned char> digits = {}; //the digits of the BigInt. The digit with the highest positional weight is the last one.
};

#endif //end of header guard BIGINT
