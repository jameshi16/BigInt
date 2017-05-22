#include <iostream>
#include "BigInt.h" //the whole point is to test this, after all.

int main()
{
  BigInt bi("123456789123456789"); //a number larger than max_int

  bi.quickPrint(); //prints the large number
}
