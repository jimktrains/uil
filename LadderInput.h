#pragma once
#include "Pentastate.h"

/**
 * Represents an input into the system.
 */
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


class SemiRandomInput : public LadderInput
{
  Pentastate i;
  public:
    SemiRandomInput();
    Pentastate value();
};



SemiRandomInput :: SemiRandomInput() :
  i(fromInt(rand() % 4))
{
}
Pentastate SemiRandomInput :: value()
{
  return i;
}

class FixedInput : public LadderInput
{
  Pentastate i;
  public:
    SemiRandomInput(Pentastate input);
    Pentastate value();
};

FixedInput :: SemiRandomInput(Pentastate input) :
  i(input)
{
}
Pentastate FixedInput :: value()
{
  return i;
}
