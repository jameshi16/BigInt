#include <iostream>
#include <limits>

#include "BigInt.h" //the whole point is to test this, after all.

void pause()
{
  char x;
  std::cin >> x; //pause
}

/* This main function will test the BigInt against every number that a long long can store */
int main()
{
  std::cout << "Numerical vs Non-Numerical test: " << std::boolalpha << (BigInt(100) == BigInt("100")) << std::noboolalpha << std::endl;
  std::cout << "Welcome to the BigInt test suite." << std::endl;
  std::cout << "Press 1 and hit enter to continue. Otherwise, press any other key and hit enter to exit." << std::endl;

  char input;
  std::cin >> input; //enters the input

  if (input != '1')
    return 1; //returns

  /* The loop to test every single number */
  for (unsigned long long loop = std::numeric_limits<unsigned long long>::min(); loop < std::numeric_limits<unsigned long long>::max(); loop++)
  {
    for (unsigned long long innerLoop = std::numeric_limits<unsigned long long>::min(); innerLoop < std::numeric_limits<unsigned long long>::max(); innerLoop++)
    {
      std::cout << "Test (" << loop << "," << innerLoop << "): ";
      BigInt firstNumber = loop;
      BigInt secondNumber = innerLoop;

      if (firstNumber + secondNumber != BigInt(loop + innerLoop)) //check addition
      {
        std::cout << "FAILED (Addition), ";
        (firstNumber + secondNumber).print();
        std::cout << " vs ";
        BigInt(loop + innerLoop).print();
        std::cout << ", ";
        pause();
      }

      if (loop >= innerLoop)
      {
        if (firstNumber - secondNumber != BigInt(loop - innerLoop)) //check subtraction
        {
          std::cout << "FAILED (Subtraction), ";
          (firstNumber - secondNumber).print();
          std::cout << " vs ";
          BigInt(loop - innerLoop).print();
          std::cout << ", ";
          pause();
        }
      }

      if (firstNumber * secondNumber != BigInt(loop * innerLoop)) //check multiplication
      {
        std::cout << "FAILED (Multiplication), ";
        (firstNumber * secondNumber).print();
        std::cout << " vs ";
        BigInt(loop * innerLoop).print();
        std::cout << ", ";
        pause();
      }

      if (secondNumber != 0 && innerLoop != 0)
      {
        if (firstNumber / secondNumber != BigInt(loop / innerLoop))
        {
          std::cout << "FAILED (Division), ";
          (firstNumber / secondNumber).print();
          std::cout << " vs ";
          BigInt(loop / innerLoop).print();
          std::cout << ", ";
          pause();
        }

        if (firstNumber % secondNumber != BigInt(loop % innerLoop))
        {
          std::cout << "FAILED (Modulus), ";
          (firstNumber % secondNumber).print();
          std::cout << " vs ";
          BigInt(loop % innerLoop).print();
          std::cout << ", ";
          pause();
        }
      }

      std::cout << "PASSED, ";
    }

  }
}
