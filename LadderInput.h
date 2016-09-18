#pragma once
#include "Pentastate.h"

class LadderInput
{
  public:
    Pentastate value();
};

Pentastate LadderInput :: value()
{
  int i = rand() % 4;
  std::cout << "->" << toString(fromInt(i)) << std::endl;
  return fromInt(i);
}


