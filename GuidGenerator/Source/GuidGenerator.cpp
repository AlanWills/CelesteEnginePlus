// GuidGenerator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Guid.hpp"


int main()
{
  char c = 0;
  while (c != 'q')
  {
    std::cout << xg::newGuid() << std::endl;
    std::cout << "Press 'q' to quit or any other button to generate again..." << std::endl;
    std::cin.get(c);
  }

  return 0;
}

