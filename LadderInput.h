#pragma once
#include "Pentastate.h"

class LadderInput
{
  public:
    virtual Pentastate value() = 0;
    virtual ~LadderInput() {};
};

class RandomInput : public LadderInput
{
  public:
    Pentastate value();
};

Pentastate RandomInput :: value()
{
  int i = rand() % 4;
  std::cout << "->" << toString(fromInt(i)) << std::endl;
  return fromInt(i);
}


