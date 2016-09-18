#pragma once
#include "LadderInput.h"

class LadderOutput : public LadderInput
{
  public:
    void setValue(Pentastate value);
};

void LadderOutput :: setValue(Pentastate value)
{
  std::cout << "<-" << toString(value) << std::endl;
}
