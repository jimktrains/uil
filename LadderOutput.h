#pragma once
#include "LadderInput.h"

class LadderOutput : public LadderInput
{
  public:
    virtual void setValue(Pentastate value) = 0;
};

class TestOutput : public LadderOutput 
{
  Pentastate val;
  public:
    void setValue(Pentastate value);
    Pentastate value();
    TestOutput();
};

TestOutput :: TestOutput() :
  val(Pentastate::None)
{}

void TestOutput :: setValue(Pentastate value)
{
  val = value;
  std::cout << "<-" << toString(value) << std::endl;
}

Pentastate TestOutput :: value()
{
  return val;
}
