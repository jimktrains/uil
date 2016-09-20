#include "Pentastate.h"

bool PentastateLogic::isHigh(Pentastate a)
{
  return (a == Pentastate::High || a == Pentastate::Rising);
}

bool  PentastateLogic::isLow(Pentastate a)
{
  return (a == Pentastate::Low || a == Pentastate::Falling);
}

Pentastate  PentastateLogic::INV(const Pentastate a)
{
  switch(a) {
    case Pentastate::None: return Pentastate::None;
    case Pentastate::Low: return Pentastate::High;
    case Pentastate::Falling: return Pentastate::Rising;
    case Pentastate::Rising: return Pentastate::Falling;
    case Pentastate::High: return Pentastate::Low;
    // Satisfies the compiler
    default: return Pentastate::None;
  }
}

Pentastate  PentastateLogic::AND(const Pentastate a, const Pentastate b)
{
  if (isHigh(a) && isHigh(b))
  {
    return Pentastate::High;
  }

  return Pentastate::Low;
}

Pentastate PentastateLogic::OR(const Pentastate a,const Pentastate b)
{
  if (isHigh(a) || isHigh(b))
  {
    return Pentastate::High;
  }

  return Pentastate::Low;
}

Pentastate PentastateLogic::XOR(const Pentastate a, const Pentastate b)
{
  return AND(OR(a,b),INV(AND(a,b)));
}

Pentastate PentastateLogic::ANDP(const Pentastate a, const Pentastate b)
{
  if (a == Pentastate::Rising)
  {
    if (b == Pentastate::Rising)
    {
      return Pentastate::Rising;
    }
  }

  return Pentastate::Low;
}

Pentastate PentastateLogic::ORP(const Pentastate a,const Pentastate b)
{
  if (a == Pentastate::Rising)
  {
    return Pentastate::Rising;
  }
  if (b == Pentastate::Rising)
  {
    return Pentastate::Rising;
  }

  return Pentastate::Low;
}

Pentastate PentastateLogic::XORP(const Pentastate a, const Pentastate b)
{
  return ANDP(ORP(a,b),INV(ANDP(a,b)));
}

Pentastate PentastateLogic::ANDF(const Pentastate a, const Pentastate b)
{
  if (a == Pentastate::Falling)
  {
    if (b == Pentastate::Falling)
    {
      return Pentastate::Falling;
    }
  }

  return Pentastate::Low;
}

Pentastate PentastateLogic::ORF(const Pentastate a,const Pentastate b)
{
  if (a == Pentastate::Falling)
  {
    return Pentastate::Falling;
  }
  if (b == Pentastate::Falling)
  {
    return Pentastate::Falling;
  }

  return Pentastate::Low;
}

Pentastate PentastateLogic::XORF(const Pentastate a, const Pentastate b)
{
  return ANDF(ORF(a,b),INV(ANDF(a,b)));
}

Pentastate PentastateLogic::fromInt(const int i)
{
  switch(i)
  {
    case 0: return Pentastate::Low;
    case 1: return Pentastate::Falling;
    case 2: return Pentastate::Rising;
    case 3: return Pentastate::High;
    // Satisfy the compiler
    default: return Pentastate::None;
  }
}

const char *PentastateLogic::toString(Pentastate x)
{
  switch(x)
  {
    case Pentastate::None: return "None";
    case Pentastate::Low: return "Low";
    case Pentastate::Falling: return "Falling";
    case Pentastate::Rising: return "Rising";
    case Pentastate::High: return "High";
    // Satisfy the compiler
    default: return "error";
  }
}
